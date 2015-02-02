/*
 * IMU.cpp
 *
 *  Created on: 25.11.2014
 *      Author: holger
 */

#include "IMU.h"
#include "stdlib.h"

RawVector3D minimum = { 32767, 32767, 32767 };
RawVector3D maximum = { -32767, -32767, -32767 };

Topic<RawVector3D> accTopic(-1, "accelerometer");
Topic<RawVector3D> magTopic(-1, "magnetometer");

Topic<Vector3D> gyroTopic(-1, "gyro");

Topic<float> tempTopic(-1, "temperature");
Topic<float> orientationTopic(-1, "orientation");
Topic<float> yawAngTopic(-1, "gyro yaw angle");

#define IMU_PORT GPIO_055 //=PD07

uint32_t retVal = 0;

const uint8_t magDataCmd[] = { 0x80 | MAG_X_L };
const uint8_t tmpDataCmd[] = { 0x80 | TEMP_LOW };

RawVector3D tempMagDat;
float gyroZValue = 0.0;

int16_t min(int16_t a, int16_t b){
	if(a > b)
		return b;
	return a;
}

int16_t max(int16_t a, int16_t b){
	if(a > b)
		return a;
	return b;
}

IMU::IMU(const char* name, uint64_t periode) : Thread(name){
	this->periode = periode;
	this->magCalibIsActive = false;

}

void IMU::setMagCalibMode(bool active){
	magCalibIsActive = active;
}

void IMU::init(){

	xprintf("IMU init period: %ld\n", periode);

	imuGPIO = HAL_GPIO(IMU_PORT);
	imuGPIO.init(true,1,1);

	int32_t error = imuI2C.init();

	if(error == 0)
		xprintf("\ni2c IMU successfully initialised\n");
	else
		xprintf("\n## i2c IMU ERROR@init ##\n");

	gyro = Gyro();
	gyro.init();

	acc = Acc();
	acc.init();
}

bool IMU::writeReadCheck(uint8_t slave_address, uint8_t address, uint8_t cmd){
	uint8_t cmd_arr[2] = { address, cmd };
	uint8_t buf[1] = { 0 };
	retVal = imuI2C.writeRead(slave_address, cmd_arr, 2, buf, 1);

	xprintf("writeReadCheck - retVal: %d, value: %d\n", retVal, buf[0]);

	return (retVal > 0 && buf[0] == cmd);
}

bool IMU::readCheck(uint8_t slave_address, uint8_t address, uint8_t targetValue){
	uint8_t cmd_arr[2] = { address };
	uint8_t buf[1] = { 0 };
	retVal = imuI2C.writeRead(slave_address, cmd_arr, 1, buf, 1);

	xprintf("writeReadCheck - retVal: %d, value: %d\n", retVal, buf[0]);

	return (retVal > 0 && buf[0] == targetValue);
}


void IMU::calcBias(void){
	/* ACCELEROMETER */
	long	x_sum = 0.0, y_sum = 0.0, z_sum = 0.0,
			xAccSum = 0.0, yAccSum = 0.0, zAccSum = 0.0,
			xMagSum = 0.0,yMagSum = 0.0, zMagSum = 0.0 ;

	for(uint8_t i = 0; i < BIAS_BUFFER_SIZE; i++){
		gyro.read();

		x_sum += gyro.getX();
		y_sum += gyro.getY();
		z_sum += gyro.getZ();

		acc.read();

		xAccSum += acc.getAccX();
		yAccSum += acc.getAccY();
		zAccSum += acc.getAccZ();

		xMagSum += acc.getMagX();
		yMagSum += acc.getMagY();
		zMagSum += acc.getMagZ();

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);
	}

	gyro.setBias(x_sum / BIAS_BUFFER_SIZE,
			y_sum / BIAS_BUFFER_SIZE,
			z_sum / BIAS_BUFFER_SIZE);

	acc.setAccBias(xAccSum / BIAS_BUFFER_SIZE,
			yAccSum / BIAS_BUFFER_SIZE,
			zAccSum / BIAS_BUFFER_SIZE);

	/*
	acc.setMagBias(xMagSum / BIAS_BUFFER_SIZE,
			yMagSum / BIAS_BUFFER_SIZE,
			zMagSum / BIAS_BUFFER_SIZE);
	*/
	if (DEBUG){
		xprintf("gyro bias: [%ld|%ld|%ld]\n", gyro.getXBias(), gyro.getYBias(), gyro.getZBias());
		xprintf("acc bias: [%ld|%ld|%ld]\n", acc.getAccXBias(), acc.getAccYBias(), acc.getAccZBias());
		xprintf("mag bias: [%ld|%ld|%ld]\n", acc.getMagXBias(), acc.getMagYBias(), acc.getMagZBias());
	}
}
void IMU::setPeriode(uint64_t periode){
	this->periode = periode;
	if (DEBUG) xprintf("changed IMU period to: %ld\n", this->periode);
}

void IMU::run(){
	RawVector3D accRawData, magRawData;
	Vector3D gyroRawData;
	float temp, orientation;

	calcBias();

	while(1){
		suspendCallerUntil(NOW() + periode);

		gyro.read();
		gyroRawData.x = gyro.getX();
		gyroRawData.y = gyro.getY();
		gyroRawData.z = gyro.getZ();

		acc.read();
		accRawData.x = acc.getAccX();
		accRawData.y = acc.getAccY();
		accRawData.z = acc.getAccZ();

		tempMagDat.x = acc.getMagX();
		tempMagDat.y = acc.getMagY();
		tempMagDat.z = acc.getMagZ();

		if(magCalibIsActive){

			minimum.x = min(tempMagDat.x, minimum.x);
			minimum.y = min(tempMagDat.y, minimum.y);
			minimum.z = min(tempMagDat.z, minimum.z);

			maximum.x = max(tempMagDat.x, maximum.x);
			maximum.y = max(tempMagDat.y, maximum.y);
			maximum.z = max(tempMagDat.z, maximum.z);

			xprintf("min: (%d|%d|%d)\n", minimum.x, minimum.y, minimum.z);
			xprintf("max: (%d|%d|%d)\n", maximum.x, maximum.y, maximum.z);
		}


//		/* mag filter */
		if(!(abs(magRawData.x - tempMagDat.x) > 30
			|| abs(magRawData.y - tempMagDat.y) > 30
			|| abs(magRawData.z - tempMagDat.z) > 30))
		{
			magRawData.x = acc.getMagX();
			magRawData.y = acc.getMagY();
			magRawData.z = acc.getMagZ();
		}

		/* gyro integration */
		if(abs(gyroRawData.z) > 1){
			gyroZValue += gyro.getZ() * (periode / (MILLISECONDS * 1000.0));
		}
		//xprintf("GYRANG: %f (prd:%f)\n", gyroZValue, periode / (MILLISECONDS * 1000.0));

		//yawAngTopic.publish(gyroZValue);
		/*
		orientation = acc.getOrientation();
		temp = acc.getTemp();

		accTopic.publish(accRawData);
		gyroTopic.publish(gyroRawData);
		magTopic.publish(magRawData);
		orientationTopic.publish(orientation);
		tempTopic.publish(temp);

		*/

		if(DBGOUT){
			xprintf("GYRDAT%d,%d,%d\n", gyroRawData.x, gyroRawData.y, gyroRawData.z);
			xprintf("ACCDAT%d,%d,%d\n", accRawData.x, accRawData.y, accRawData.z);
			xprintf("MAGDAT%d,%d,%d\n", magRawData.x, magRawData.y, magRawData.z);
			xprintf("HEADNG%f\n", orientation);
		}
	}

}

