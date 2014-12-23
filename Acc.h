/*
 * Acc.h
 *
 *  Created on: 23.12.2014
 *      Author: holger
 */

#ifndef ACC_H_
#define ACC_H_
#include "rodos.h"
#include "hal/hal_i2c.h"
#include "hal/hal_gpio.h"
#define CS_ACC_PORT GPIO_032 //=PC00

#define ACC_SLAVE_ADDRESS 0x1D
#define ACC_CTRL_REG1 0x20
#define ACC_POWER_CMD 0b01010111 //<- 50Hz, all axes

#define ACC_CTRL_REG5 0x24
#define ACC_ACT_TEMP 0b11110000 //<- act. temp sens, hi magn res, mag data rat=50Hz
#define CS_ACC_PORT GPIO_032 //=PC00

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
#define ACC_DEVICE_ID 0x49

extern HAL_I2C imuI2C;

class Acc {
private:
	bool isOn;
	HAL_GPIO gpioAcc;

	uint8_t txBuf[3];
	uint8_t rxBuf[6];
	uint16_t txBuf16[3];
	int32_t err[2];
	uint16_t result;
public:
	void init(void);
	void stop(void);
	void read(void);
	void readTemp(void);

	void setAccBias(uint16_t x,uint16_t y,uint16_t z);
	void setMagBias(uint16_t x,uint16_t y,uint16_t z);

	uint16_t accX, accY, accZ;
	uint16_t accXBias, accYBias, accZBias;

	uint16_t magX, magY, magZ;
	uint16_t magXBias, magYBias, magZBias;

	uint16_t temp;
};

#endif /* ACC_H_ */
