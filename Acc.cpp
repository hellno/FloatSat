/*
 * Acc.cpp
 *
 *  Created on: 23.12.2014
 *      Author: holger
 */

#include "Acc.h"
#include "helper.h"

void Acc::init(void){
	//ACC INIT
	if (!isOn || gpioAcc.read() == 0) {
		gpioAcc = HAL_GPIO(CS_ACC_PORT);
		gpioAcc.init(true, 1, 1);
	}
	isOn = true;

	txBuf[0] = ACC_CTRL_REG1;
	txBuf[1] = ACC_POWER_CMD;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 2, rxBuf, 1);

	if(err[0] < 0){
		xprintf("## error @acc init ##\n");
		return;
	}
	txBuf[0] = ACC_CTRL_REG5;
	txBuf[1] = ACC_ACT_TEMP;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 2, rxBuf, 1);

	if(err[0] < 0){
		xprintf("## error @acc init ##\n");
		return;
	}

	txBuf[0] = WHO_AM_I_ADDRESS;
//	txBuf[1] = ACC_DEVICE_ID;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 1);

	if(err[0] < 0 || rxBuf[0] != ACC_DEVICE_ID){
		xprintf("## acc has wrong id ##\n");
		return;
	}
	//MAG INIT
	txBuf[0] = ACC_CTRL_REG6_M;
	txBuf[1] = 0x00;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 2, rxBuf, 1);

	if(err[0] < 0){
		xprintf("## error @acc init ##\n");
		return;
	}
	txBuf[0] = ACC_CTRL_REG7_M;
	txBuf[1] = 0x00;
	if (imuI2C.write(ACC_SLAVE_ADDRESS, txBuf, 2) < 0)
		imuI2C.init();

	txBuf[0] = ACC_CTRL_REG4;
	txBuf[1] = 0x04;
	if (imuI2C.write(ACC_SLAVE_ADDRESS, txBuf, 2) < 0)
		imuI2C.init(); // Magnetometer data ready on INT2_XM (0x08)

	txBuf[0] = INT_CTRL_REG_M;
	txBuf[1] = 0x09;
	if (imuI2C.write(ACC_SLAVE_ADDRESS, txBuf, 2) < 0)
		imuI2C.init(); // Enable interrupts for mag, active-low, push-pull

	accXBias = 0, accYBias = 0, accZBias = 0;
	magXBias = 0, magYBias = 0, magZBias = 0;
	isBiasSet = false;

	xprintf("acc init done\n");
}

void Acc::read(void){
	memset(rxBuf, 0, sizeof(rxBuf));
	memset(txBuf, 0, sizeof(txBuf));
	/* ACCELEROMETER */
	txBuf[0] = ACC_X_L | 0x80;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 6);
	if (err[0] < 0) {
		xprintf("## error while reading acc data//init IMU ##\n\n");
		memset(rxBuf, 0, sizeof(rxBuf));
	}
	accX = (int16_t) ((rxBuf[1] << 8) | rxBuf[0] - accXBias);
	accY = (int16_t) ((rxBuf[3] << 8) | rxBuf[2] - accYBias);
	accZ = (int16_t) ((rxBuf[5] << 8) | rxBuf[4] - accZBias);


	/* MAGNETOMETER */
	txBuf[0] = MAG_X_L | 0x80;
	err[0] = imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 6);
	if (err[0] < 0) {
		xprintf("## error while reading mag data//init IMU ##\n\n");
		memset(rxBuf, 0, sizeof(rxBuf));
	}
	magX = (int16_t) ((rxBuf[1] << 8) | rxBuf[0] - magXBias);
	magY = (int16_t) ((rxBuf[3] << 8) | rxBuf[2] - magYBias);
	magZ = (int16_t) ((rxBuf[5] << 8) | rxBuf[4] - magZBias);
}

void Acc::readTemp(void){
	memset(rxBuf, 0, sizeof(rxBuf));
	memset(txBuf, 0, sizeof(txBuf));

	txBuf[0] =  0x80 | TEMP_LOW;

	imuI2C.writeRead(ACC_SLAVE_ADDRESS, txBuf, 1, rxBuf, 2);
	temp = REFTEMP - (rxBuf[0] << 4 + rxBuf[1]);
}

void Acc::setAccBias(uint16_t x,uint16_t y,uint16_t z){
	accXBias = x;
	accYBias = y;
	accZBias = z;

	isBiasSet = true;
}

void Acc::setMagBias(uint16_t x,uint16_t y,uint16_t z){
	magXBias = x;
	magYBias = y;
	magZBias = z;

	isBiasSet = true;
}

uint16_t Acc::getTemp(void){
	return temp;
}

int16_t Acc::getAccX(void){
	return accX - accXBias;
}

int16_t Acc::getAccY(void){
	return accY - accYBias;
}

int16_t Acc::getAccZ(void){
	return accZ - accZBias;
}

int16_t Acc::getMagX(void){
	return magX - magXBias;
}

int16_t Acc::getMagY(void){
	return magY - magYBias;
}

int16_t Acc::getMagZ(void){
	return magZ - magZBias;
}

int16_t Acc::getAccXBias(void){
	return accXBias;
}
int16_t Acc::getAccYBias(void){
	return accYBias;
}
int16_t Acc::getAccZBias(void){
	return accZBias;
}
int16_t Acc::getMagXBias(void){
	return magXBias;
}
int16_t Acc::getMagYBias(void){
	return magYBias;
}
int16_t Acc::getMagZBias(void){
	return magZBias;
}
float Acc::getOrientation(void){
	Vector3D m = (Vector3D){getMagX(), getMagY(), getMagZ()};
	Vector3D a = (Vector3D){getAccX(), getAccY(), getAccZ()};

	Vector3D from = (Vector3D){0,-1,0};

	//	 shift and scale
	m.x = (m.x - MAG_MIN_X) / (MAG_MAX_X - MAG_MIN_X) * 2 - 1.0;
    m.y = (m.y - MAG_MIN_Y) / (MAG_MAX_Y - MAG_MIN_Y) * 2 - 1.0;
    m.z = (m.z - MAG_MIN_Z) / (MAG_MAX_Z - MAG_MIN_Z) * 2 - 1.0;

	vector_normalize(&a);
	// compute E and N
	Vector3D E;
	Vector3D N;
	vector_cross(&m, &a, &E);
	vector_normalize(&E);
	vector_cross(&a, &E, &N);

	// compute heading
	double heading = atan2(vector_dot(&E, &from), vector_dot(&N, &from)) * 180 / M_PI;
	if (heading < 0)
		heading += 360;
	return heading;
}
void Acc::stop(void){
	if (isOn & gpioAcc.read() == 1) {
		gpioAcc.init(false, 1, 1);
	}
	isOn = false;
}
