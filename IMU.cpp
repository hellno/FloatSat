/*
 * IMU.cpp
 *
 *  Created on: 25.11.2014
 *      Author: holger
 */

#include "IMU.h"

#include <cstdlib>

#define DEBUG true

Topic<RawVector3D> accTopic(-1, "accelerometer");
Topic<RawVector3D> magTopic(-1, "magnetometer");
Topic<RawVector3D> gyroTopic(-1, "gyro");
Topic<uint8_t> tempTopic(-1, "temperature");
#define RAW_DATA_BUFFER_SIZE 5

#define LSM303DLH_ACC_ADDR  0x18
#define LSM303DLH_MAG_ADDR  0x1E
#define L3G4200D_GYR_ADDR   0x69

#define ACC_SLAVE_ADDRESS 0x1D
#define MAG_SLAVE_ADDRESS 0x1D
#define GYRO_SLAVE_ADDRESS 0x6B

#define GYRO_POWER_ADDRESS 0x24
#define GYRO_POWER_CMD 0x80

#define GYRO_STATUS 0x27

#define ACC_CTRL_REG1 0x20
#define ACC_POWER_CMD 0b01010111 //<- 50Hz, all axes

#define ACC_CTRL_REG5 0x24
#define ACC_ACT_TEMP 0b11110000 //<- act. temp sens, hi magn res, mag data rat=50Hz

#define GYRO_SENSITIVITY 8.75f
#define ACC_SENSITIVITY 0.061f
#define MAG_SENSITIVITY 0.08f

#define GYRO_RAWDATA_FACTOR (GYRO_SENSITIVITY / RAW_DATA_BUFFER_SIZE)

#define IMU_PORT GPIO_055 //=PD07
#define CS_GYRO_PORT GPIO_018 //=PB02
#define CS_ACC_PORT GPIO_032 //=PC00

#define GYRO_X_L 0x28
#define GYRO_X_H 0x29
#define GYRO_Y_L 0x2A
#define GYRO_Y_H 0x2B
#define GYRO_Z_L 0x2C
#define GYRO_Z_H 0x2D

#define MAG_X_L 0x08
#define MAG_X_H 0x09
#define MAG_Y_L 0x0A
#define MAG_Y_H 0x0B
#define MAG_Z_L 0x0C
#define MAG_Z_H 0x0D

#define ACC_X_L 0x28
#define ACC_X_H 0x29
#define ACC_Y_L 0x2A
#define ACC_Y_H 0x2B
#define ACC_Z_L 0x2C
#define ACC_Z_H 0x2D

#define TEMP_LOW 0x05
#define TEMP_HIGH 0x06

#define WHO_AM_I_ADDRESS 0x0F
#define GYRO_DEVICE_ID 0xD4
#define ACC_DEVICE_ID 0x49

uint32_t retVal = 0;

const uint8_t who_am_i[] = { WHO_AM_I_ADDRESS };
const uint8_t readDataCmd[] = { 0x80 | GYRO_X_L };
const uint8_t magDataCmd[] = { 0x80 | MAG_X_L };
const uint8_t tmpDataCmd[] = { 0x80 | TEMP_LOW };
const uint8_t readGyroManually[] = { GYRO_X_L, GYRO_X_H,
							GYRO_Y_L, GYRO_Y_H,
							GYRO_Z_L, GYRO_Z_H };
RawVector3D gyroOffset;

IMU::IMU(const char* name, HAL_I2C *i2c){
	this->i2c = i2c;
}


void IMU::init(){
	imuGPIO = HAL_GPIO(IMU_PORT);
	gyroGPIO = HAL_GPIO(CS_GYRO_PORT);
	accGPIO = HAL_GPIO(CS_ACC_PORT);

	imuGPIO.init(true,1,1);
	gyroGPIO.init(true,1,1);
	accGPIO.init(true,1,1);

	xprintf("start IMU init\n");
	//xprintf("GPIOPINS: imu %d, gyro %d, acc %d\n", imuGPIO.readPins(), gyroGPIO.readPins(), accGPIO.readPins());

	int32_t error = i2c->init();

	if(error == 0)
		xprintf("i2c IMU successfully initialised\n");
	else
		xprintf("i2c IMU ERROR@init\n");

	initGyro();
	initAcc();
}

bool IMU::writeReadCheck(uint8_t slave_address, uint8_t address, uint8_t cmd){
	uint8_t cmd_arr[2] = { address, cmd };
	uint8_t buf[1] = { 0 };
	retVal = i2c->writeRead(slave_address, cmd_arr, 2, buf, 1);

	//xprintf("writeReadCheck - retVal: %d,buf: %d\n", retVal, buf[0]);

	return (retVal > 0 && buf[0] == cmd);
}

bool IMU::readCheck(uint8_t slave_address, uint8_t address, uint8_t targetValue){
	uint8_t cmd_arr[2] = { address };
	uint8_t buf[1] = { 0 };
	retVal = i2c->writeRead(slave_address, cmd_arr, 1, buf, 1);

	//xprintf("writeReadCheck - retVal: %d,buf: %d\n", retVal, buf[0]);

	return (retVal > 0 && buf[0] == targetValue);
}

void IMU::initGyro(){
	//give it up for the gyro
	if(!writeReadCheck(GYRO_SLAVE_ADDRESS, GYRO_POWER_ADDRESS, GYRO_POWER_CMD)){
		xprintf("error @gyro init\n");
		return;
	}

	//check status
	if(!writeReadCheck(GYRO_SLAVE_ADDRESS, GYRO_STATUS, 0)){
		xprintf("gyro status wrong!, retVal: %d\n", retVal);
		return;
	}

	if(!writeReadCheck(GYRO_SLAVE_ADDRESS, WHO_AM_I_ADDRESS, GYRO_DEVICE_ID)){
		xprintf("gyro has wrong id\n");
		return;
	}

	xprintf("gyro successfully initiated\n");
	xprintf("GYRO_FACTOR %f\n", GYRO_RAWDATA_FACTOR);

	xprintf("GYRO OFFSET: [%d,%d,%d]\n", gyroOffset.x, gyroOffset.y, gyroOffset.z);
}


void IMU::initAcc(){
	//start the spaceship, accelerometer
	if(!writeReadCheck(ACC_SLAVE_ADDRESS, ACC_CTRL_REG1, ACC_POWER_CMD)){
		xprintf("error @acc init\n");
		return;
	}

	if(!writeReadCheck(ACC_SLAVE_ADDRESS, ACC_CTRL_REG5, ACC_ACT_TEMP)){
		xprintf("error @acc init\n");
		return;
	}

	if(!writeReadCheck(ACC_SLAVE_ADDRESS, WHO_AM_I_ADDRESS, ACC_DEVICE_ID)){
		xprintf("acc has wrong id\n");
		return;
	}

	xprintf("acc successfully initiated\n");

}

void IMU::readMagData(uint8_t* buf){
	i2c->writeRead(ACC_SLAVE_ADDRESS, magDataCmd, 1, buf, 6);
}

void IMU::readGyroData(uint8_t *buf){
	i2c->writeRead(GYRO_SLAVE_ADDRESS, readDataCmd, 1, buf, 6);
}

void IMU::readAccData(uint8_t *buf){
	i2c->writeRead(ACC_SLAVE_ADDRESS, readDataCmd, 1, buf, 6);
}

void IMU::mergeRawData(uint8_t *from, RawVector3D to){
	//must be in format XL,XH - YL,YH - ZL,ZH
	to.x = (from[0] << 8 + from[1]);
	to.y = (from[2] << 8 + from[3]);
	to.z = (from[4] << 8 + from[5]);
}

void IMU::run(){

	uint8_t dataCounter = 0;

	uint8_t gyroBuf[6] = { 0 };
	uint8_t accBuf[6] = { 0 } ;
	uint8_t magBuf[6] = { 0 };

	uint8_t tempBuf[2] =  { 0 };
	uint8_t temperature = 0;

	RawVector3D gyroRawData, magRawData, accRawData;
	Vector3D gyroData;

	TIME_LOOP(0, 250 * MILLISECONDS){

		//readGyroData(gyroBuf);
		//mergeRawData(gyroBuf, gyroRawData);

		i2c->writeRead(GYRO_SLAVE_ADDRESS, readDataCmd, 1, gyroBuf, 6);
		gyroRawData.x = (uint16_t)(gyroBuf[0] << 8 + gyroBuf[1]);
		gyroRawData.y = (uint16_t)(gyroBuf[2] << 8 + gyroBuf[3]);
		gyroRawData.z = (uint16_t)(gyroBuf[4] << 8 + gyroBuf[5]);

		if(DEBUG) xprintf("GYRDAT%d,%d,%d,\n", gyroRawData.x, gyroRawData.y, gyroRawData.z);

		i2c->writeRead(MAG_SLAVE_ADDRESS, magDataCmd, 1, magBuf, 6);
		magRawData.x = (uint16_t)(magBuf[0] << 8 + magBuf[1]);
		magRawData.y = (uint16_t)(magBuf[2] << 8 + magBuf[3]);
		magRawData.z = (uint16_t)(magBuf[4] << 8 + magBuf[5]);

		if(DEBUG) xprintf("mag  [%d|%d|%d]\n", magRawData.x, magRawData.y, magRawData.z);

		i2c->writeRead(ACC_SLAVE_ADDRESS, readDataCmd, 1, accBuf, 6);
		accRawData.x = (uint16_t)(accBuf[0] << 8 + accBuf[1]);
		accRawData.y = (uint16_t)(accBuf[2] << 8 + accBuf[3]);
		accRawData.z = (uint16_t)(accBuf[4] << 8 + accBuf[5]);

		if(DEBUG) xprintf("ACCDAT%d,%d,%d\n", accRawData.x, accRawData.y, accRawData.z);

		i2c->writeRead(ACC_SLAVE_ADDRESS, tmpDataCmd, 1, tempBuf, 2);
		temperature = (tempBuf[0] << 4 + tempBuf[1]) / 8;

		if(DEBUG) xprintf("tmp  [%d]\n", temperature);

		if(!DEBUG) xprintf("CURBAT%d\n", rand() % 5 + 1);

		accTopic.publish(accRawData);
		gyroTopic.publish(gyroRawData);
		magTopic.publish(magRawData);
		tempTopic.publish(temperature);

	}

}
//GYRO OFFSET STUFF
//xprintf("starting bias calc..");
//
//	RawVector3D rawVectors[500];
//	long x_sum = 0, y_sum = 0, z_sum = 0;
//
//	for(int i = 0; i < 500; i++){
//		readGyroData(gyroBuf);
//
//		x_sum += (gyroBuf[0] << 8 + gyroBuf[1]);
//		y_sum += (gyroBuf[2] << 8 + gyroBuf[3]);
//		z_sum += (gyroBuf[4] << 8 + gyroBuf[5]);
//
//		xprintf("calcOffset: x:%d, y:%d, z:%d\n", x_sum, y_sum, z_sum);
//
//		suspendCallerUntil(NOW() + 2*MILLISECONDS);
//
//	}
//	gyroOffset.x = x_sum / 500;
//	gyroOffset.y = y_sum / 500;
//	gyroOffset.z = z_sum / 500;
//
//	xprintf("calcOffset: x:%d, y:%d, z:%d\n", gyroOffset.x, gyroOffset.y, gyroOffset.z);


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

