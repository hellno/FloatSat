/*
 * IMU.cpp
 *
 *  Created on: 25.11.2014
 *      Author: holger
 */

#include "IMU.h"

Topic<SatState> st(-1, "SatState");

#define LSM303DLH_ACC_ADDR  0x18
#define LSM303DLH_MAG_ADDR  0x1E
#define L3G4200D_GYR_ADDR   0x69

#define ACC_SLAVE_ADDRESS 0x1D
#define MAGN_SLAVE_ADDRESS 0x1D
#define GYRO_SLAVE_ADDRESS 0x6B

#define GYRO_POWER_ADDRESS 0x24
#define GYRO_POWER_CMD 0x80

#define GYRO_FIFO_CTRL_REG 0x2E
#define GYRO_STREAM_FIFO_MODE 0b01100000

#define GYRO_CTRL_REG5_G 0x24
#define GYRO_REG5_VAL 0b01000000

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

IMU::IMU(const char* name, HAL_I2C *i2c,  uint64_t periode){
	this->i2c = i2c;
	this->periode = periode;

	this->readDataCmd[0] = GYRO_X_L;
	this->magDataCmd[0] = MAG_X_L;
}
void IMU::init(){
	imuGPIO = HAL_GPIO(IMU_PORT);
	gyroGPIO = HAL_GPIO(CS_GYRO_PORT);
	accGPIO = HAL_GPIO(CS_ACC_PORT);

	imuGPIO.init(true,1,1);
	gyroGPIO.init(true,1,1);
	accGPIO.init(true,1,1);

	xprintf("start IMU init\n");
	xprintf("GPIOPINS: imu %d, gyro %d, acc %d\n", imuGPIO.readPins(), gyroGPIO.readPins(), accGPIO.readPins());

	int32_t error = i2c->init();
	int32_t retVal = 0;

	if(error == 0)
		xprintf("i2c IMU successfully initialised\n");
	else
		xprintf("i2c IMU ERROR@init\n");

	uint8_t who_am_i[1] = { WHO_AM_I_ADDRESS };
	uint8_t buf[1] = { 0 };

	uint8_t gyro_activate[2] = { 0x20, 0b0001111 };

//	fang an zu messen, gyro!
//	retVal = i2c->write(GYRO_SLAVE_ADDRESS, gyro_activate, 1);
//	xprintf("i2cA gyro powerUp1 retVal: %d \n", retVal);

//	retVal = i2c->write(GYRO_SLAVE_ADDRESS, gyro_activate, 2);
//	xprintf("i2cA gyro powerUp2 retVal: %d \n", retVal);

	retVal = i2c->writeRead(GYRO_SLAVE_ADDRESS, who_am_i, 1, buf, 1);

	if(retVal > 0 && buf[0] == GYRO_DEVICE_ID)
		xprintf("gyro successfully initiated\n");

	buf[0] = 0;
	retVal = i2c->writeRead(ACC_SLAVE_ADDRESS, who_am_i, 1, buf, 1);

	if(retVal > 0 && buf[0] == ACC_DEVICE_ID)
		xprintf("xm successfully initiated\n");

	initGyro();

}

bool IMU::writeReadCheck(uint8_t slave_address, uint8_t address, uint8_t cmd){
	uint8_t cmd_arr[2] = { address, cmd };
	uint8_t buf[1] = { 0 };
	uint32_t retVal = i2c->writeRead(slave_address, cmd_arr, 2, buf, 1);

	//xprintf("writeReadCheck - retVal: %d,buf: %d\n", retVal, buf[0]);

	return (retVal > 0 && buf[0] == cmd);
}

void IMU::initGyro(){
	//activeate fifo
	writeReadCheck(GYRO_SLAVE_ADDRESS, GYRO_CTRL_REG5_G, GYRO_REG5_VAL);

	//set mode tto Stream-to-FIFO
	writeReadCheck(GYRO_SLAVE_ADDRESS, GYRO_FIFO_CTRL_REG, GYRO_STREAM_FIFO_MODE);

}

void IMU::readGyroData(uint8_t *buf, uint8_t bufferSize){
	if(bufferSize != 6)
		return;

	uint32_t retVal = i2c->writeRead(GYRO_SLAVE_ADDRESS, readDataCmd, 1, buf, bufferSize);
	xprintf("writeReadGyroData: %d\n", retVal);
}

void IMU::run(){
	int32_t retVal = 0;
	uint8_t buf[6] = { 0 };

	TIME_LOOP(0, periode){
		readGyroData(buf, 6);
		xprintf("xl:%d xh:%d, xl:%d xh:%d,xl:%d xh:%d\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
		xprintf("x:%d,y:%d,z:%d\n", (buf[1]<< 8) + buf[0], (buf[3]<< 8) + buf[2], (buf[5]<< 8) + buf[4]);
	}

}
