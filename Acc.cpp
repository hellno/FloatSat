/*
 * Acc.cpp
 *
 *  Created on: 23.12.2014
 *      Author: holger
 */

#include "Acc.h"

void Acc::init(void){
	if (!isOn || gpioAcc.read() == 0) {
		gpioAcc = HAL_GPIO(CS_ACC_PORT);
		gpioAcc.init(true, 1, 1);
	}
	isOn = true;

	txBuf[0] = ACC_CTRL_REG1;
	txBuf[1] = ACC_POWER_CMD;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 2, rxBuf, 1);

	if(err[0] < 0){
		xprintf("error @acc init\n");
		return;
	}
	txBuf[0] = ACC_CTRL_REG5;
	txBuf[1] = ACC_ACT_TEMP;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 2, rxBuf, 1);

	if(err[0] < 0){
		xprintf("error @acc init\n");
		return;
	}

	txBuf[0] = WHO_AM_I_ADDRESS;
//	txBuf[1] = ACC_DEVICE_ID;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 1);

	if(err[0] < 0 || rxBuf[0] != ACC_DEVICE_ID){
		xprintf("acc has wrong id\n");
		return;
	}

	xprintf("acc init done\n");
}

void Acc::read(void){
	memset(rxBuf, 0, sizeof(rxBuf));
	memset(txBuf, 0, sizeof(txBuf));
	/* ACCELEROMETER */
	txBuf[0] = ACC_X_L | 0x80;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 6);
	if (err[0] < 0) {
		xprintf("error while reading acc data//init IMU\n\n");
		memset(rxBuf, 0, sizeof(rxBuf));
	}
	accX = (rxBuf[1] << 8) | rxBuf[0];
	accY = (rxBuf[3] << 8) | rxBuf[2];
	accZ = (rxBuf[5] << 8) | rxBuf[4];


	/* MAGNETOMETER */
	txBuf[0] = MAG_X_L | 0x80;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 6);
	if (err[0] < 0) {
		xprintf("error while reading mag data//init IMU\n\n");
		memset(rxBuf, 0, sizeof(rxBuf));
	}
	magX = (rxBuf[1] << 8) | rxBuf[0] - magXBias;
	magY = (rxBuf[3] << 8) | rxBuf[2] - magYBias;
	magZ = (rxBuf[5] << 8) | rxBuf[4] - magZBias;
}

void Acc::readTemp(void){
	memset(rxBuf, 0, sizeof(rxBuf));
	memset(txBuf, 0, sizeof(txBuf));

	txBuf[0] =  0x80 | TEMP_LOW;

	imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 2);
	temp = (rxBuf[0] << 4 + rxBuf[1]) / 8;

}
void Acc::setAccBias(uint16_t x,uint16_t y,uint16_t z){
	accXBias = x;
	accYBias = y;
	accZBias = z;
}

void Acc::setMagBias(uint16_t x,uint16_t y,uint16_t z){
	magXBias = x;
	magYBias = y;
	magZBias = z;
}
void Acc::stop(void){
	if (isOn & gpioAcc.read() == 1) {
		gpioAcc.init(false, 1, 1);
	}
	isOn = false;
}
