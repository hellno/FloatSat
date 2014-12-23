/*
 * IMU.cpp
 *
 *  Created on: 25.11.2014
 *      Author: holger
 */

#include "IMU.h"

#define DEBUG true

Topic<RawVector3D> accTopic(-1, "accelerometer");
Topic<RawVector3D> magTopic(-1, "magnetometer");
Topic<RawVector3D> gyroTopic(-1, "gyro");
Topic<uint8_t> tempTopic(-1, "temperature");

#define IMU_PORT GPIO_055 //=PD07

uint32_t retVal = 0;

const uint8_t magDataCmd[] = { 0x80 | MAG_X_L };
const uint8_t tmpDataCmd[] = { 0x80 | TEMP_LOW };

RawVector3D gyroOffset;

IMU::IMU(const char* name) : Thread(name){
}


void IMU::init(){
	imuGPIO = HAL_GPIO(IMU_PORT);
	imuGPIO.init(true,1,1);

	int32_t error = imuI2C.init();

	if(error == 0)
		xprintf("\ni2c IMU successfully initialised\n");
	else
		xprintf("\ni2c IMU ERROR@init\n");

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


void IMU::calcGyroBias(void){
	long x_sum = 0.0, y_sum = 0.0, z_sum = 0.0;
	for(uint8_t i = 0; i < 100; i++){

		gyro.read();

		x_sum += gyro.x;
		y_sum += gyro.y;
		z_sum += gyro.z;

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);
	}

	x_sum /= 100, y_sum /= 100, z_sum /= 100;

	xprintf("gyro bias: [%ld|%ld|%ld]\n", x_sum, y_sum, z_sum);
	//xprintf("max uint16_t: %ld\n", UINT16_MAX);

	gyro.setBias(x_sum, y_sum, z_sum);
}

void IMU::calcAccBias(void){
	/* ACCELEROMETER */
	long xAccSum = 0.0, yAccSum = 0.0, zAccSum = 0.0,
			xMagSum = 0.0,yMagSum = 0.0, zMagSum = 0.0 ;
	for(uint8_t i = 0; i < BIAS_BUFFER_SIZE; i++){

		acc.read();

		xAccSum += acc.accX;
		yAccSum += acc.accY;
		zAccSum += acc.accZ;

		xMagSum += acc.magX;
		yMagSum += acc.magY;
		zMagSum += acc.magZ;

		suspendCallerUntil(NOW() + 10 * MILLISECONDS);
	}

	acc.setAccBias(xAccSum / BIAS_BUFFER_SIZE,
			yAccSum / BIAS_BUFFER_SIZE,
			zAccSum / BIAS_BUFFER_SIZE);

	acc.setMagBias(xMagSum / BIAS_BUFFER_SIZE,
			yMagSum / BIAS_BUFFER_SIZE,
			zMagSum / BIAS_BUFFER_SIZE);

	xprintf("acc bias: [%ld|%ld|%ld]\n", acc.accXBias, acc.accYBias, acc.accZBias);
	xprintf("mag bias: [%ld|%ld|%ld]\n", acc.magXBias, acc.magYBias, acc.magZBias);

}

void IMU::run(){
	RawVector3D gyroRawData, accRawData, magRawData;

	calcGyroBias();
	calcAccBias();

	TIME_LOOP(1 * SECONDS, 1 * SECONDS){

		gyro.read();
		gyroRawData.x = gyro.x - gyro.xBias;
		gyroRawData.y = gyro.y - gyro.yBias;
		gyroRawData.z = gyro.z - gyro.zBias;

		xprintf("GYRDAT%d,%d,%d\n", gyroRawData.x, gyroRawData.y, gyroRawData.z);

		acc.read();
		accRawData.x = acc.accX - acc.accXBias;
		accRawData.y = acc.accY - acc.accYBias;
		accRawData.z = acc.accZ - acc.accZBias;

		xprintf("ACCDAT%d,%d,%d\n", accRawData.x, accRawData.y, accRawData.z);

		magRawData.x = acc.magX - acc.magXBias;
		magRawData.y = acc.magY - acc.magYBias;
		magRawData.z = acc.magZ - acc.magZBias;

		xprintf("MAGDAT%d,%d,%d\n", magRawData.x, magRawData.y, magRawData.z);

//		accTopic.publish(accRawData);
//		gyroTopic.publish(gyroRawData);
//		magTopic.publish(magRawData);
//		tempTopic.publish(temperature);

	}

}

//GYRO INTEGRATE STUFF
//		if(dataCounter % RAW_DATA_BUFFER_SIZE == 0){
//
//			while(counter < RAW_DATA_BUFFER_SIZE){
//				tempX += gyroRawData[counter].x;
//				tempY += gyroRawData[counter].y;
//				tempZ += gyroRawData[counter].z;
//
//				counter++;
//			}
//
//			gyroData.x = tempX * GYRO_RAWDATA_FACTOR;
//			gyroData.y = tempY * GYRO_RAWDATA_FACTOR;
//			gyroData.z = tempZ * GYRO_RAWDATA_FACTOR;
//
//			xprintf("[%.2f|%.2f|%.2f]\n", gyroData.x, gyroData.y, gyroData.z);
//
//			gyroTopic.publish(gyroData);
//
//			tempX = 0.0, tempY = 0.0, tempZ = 0.0;
//			dataCounter = 0;
//			counter = 0;
//		}
//
//		dataCounter++;

