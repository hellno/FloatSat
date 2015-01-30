/*
 * Gyro.h
 *
 *  Created on: 23.12.2014
 *      Author: holger
 */

#ifndef GYRO_H_
#define GYRO_H_

#include "rodos.h"
#include "hal/hal_gpio.h"
#include "hal/hal_i2c.h"

#define GYRO_SLAVE_ADDRESS 0x6B
#define CS_GYRO_PORT GPIO_018 //=PB02

#define GYRO_POWER_ADDRESS 0x24
#define GYRO_POWER_CMD 0x80

#define CTRL_REG1_G	0x20
#define CTRL_REG2_G	0x21
#define CTRL_REG3_G	0x22
#define CTRL_REG4_G	0x23
#define CTRL_REG5_G	0x24
#define REFERENCE_G	0x25
#define GYRO_STATUS 0x27
#define GYRO_X_L 0x28
#define GYRO_X_H 0x29
#define GYRO_Y_L 0x2A
#define GYRO_Y_H 0x2B
#define GYRO_Z_L 0x2C
#define GYRO_Z_H 0x2D

#define FIFO_CTRL_REG_G	0x2E
#define FIFO_SRC_REG_G	0x2F
#define INT1_CFG_G	0x30
#define INT1_SRC_G	0x31
#define INT1_THS_XH_G	0x32
#define INT1_THS_XL_G	0x33
#define INT1_THS_YH_G	0x34
#define INT1_THS_YL_G	0x35
#define INT1_THS_ZH_G	0x36
#define INT1_THS_ZL_G	0x37
#define INT1_DURATION_G	0x38

#define GYRO_DEVICE_ID 0xD4

#define dps500Scale 0.0175f
#define dps2000Scale 0.070f

#define dps500Bias -16.3821f
#define dps2000Bias -8.684791496320523f

extern HAL_I2C imuI2C;

class Gyro {
private:
	bool isOn;
	HAL_GPIO gpioGyro;

	uint8_t txBuf[3];
	uint8_t rxBuf[6];
	uint16_t txBuf16[3];
	int32_t err[2];
	uint16_t result;
	float scale;

	int16_t x, y, z;
	int16_t xBias, yBias, zBias;
public:
	void init(void);
	void stop(void);
	void enableFifo(void);
	void disableFifo(void);
	void read(void);
	void enableStreamMode(void);
	void enableByPassMode(void);
	void setBias(uint16_t x,uint16_t y,uint16_t z);

	uint8_t readNumberOfSamples(void);
	uint8_t getModel(void);

	float getX(void);
	float getY(void);
	float getZ(void);

	int16_t getXBias(void);
	int16_t getYBias(void);
	int16_t getZBias(void);

	int16_t getZWithoutBias(void);
};

#endif /* GYRO_H_ */
