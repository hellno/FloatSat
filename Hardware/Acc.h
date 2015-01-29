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

#define ACC_CTRL_REG4 0x23
#define ACC_CTRL_REG5 0x24
#define ACC_CTRL_REG6_M 0x25
#define ACC_CTRL_REG7_M 0x26
#define INT_CTRL_REG_M 0x12

#define OLD_ACC_ACT_TEMP 0b11000000 //<- temp sens ON
#define ACC_ACT_TEMP 0b11110100 //<- temp sens ON, hi magn res, mag data rat=50Hz
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

#define REFTEMP	32

#define WHO_AM_I_ADDRESS 0x0F
#define ACC_DEVICE_ID 0x49

/* old values
#define MAG_MAX_X 244
#define MAG_MAX_Y 627
#define MAG_MAX_Z 294
#define MAG_MIN_X -600
#define MAG_MIN_Y -330
#define MAG_MIN_Z -657
*/
/* new mag values
min: (-7194|-562|-7053)
max: (660|6922|-2779)
*/
#define MAG_MAX_X 660
#define MAG_MAX_Y 6922
#define MAG_MAX_Z 2779
#define MAG_MIN_X -7194
#define MAG_MIN_Y -562
#define MAG_MIN_Z -7053

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

	bool isBiasSet;

	int16_t accX, accY, accZ;
	int16_t accXBias, accYBias, accZBias;

	int16_t magX, magY, magZ;
	int16_t magXBias, magYBias, magZBias;

	uint16_t temp;
public:
	void init(void);
	void stop(void);
	void read(void);
	void readTemp(void);

	void setAccBias(uint16_t x,uint16_t y,uint16_t z);
	void setMagBias(uint16_t x,uint16_t y,uint16_t z);
	float getOrientation(void);
	bool biasIsSet(void);

	int16_t getAccX(void);
	int16_t getAccY(void);
	int16_t getAccZ(void);
	int16_t getMagX(void);
	int16_t getMagY(void);
	int16_t getMagZ(void);

	int16_t getAccXBias(void);
	int16_t getAccYBias(void);
	int16_t getAccZBias(void);
	int16_t getMagXBias(void);
	int16_t getMagYBias(void);
	int16_t getMagZBias(void);

	uint16_t getTemp(void);
};

#endif /* ACC_H_ */
