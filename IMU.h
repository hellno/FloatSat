/*
 * IMU.h
 *
 *  Created on: 25.11.2014
 *      Author: holger
 */

#ifndef IMU_H_
#define IMU_H_

#include "rodos.h"
#include "topics.h"
#include "hal/hal_i2c.h"
#include "hal/hal_gpio.h"

class IMU  : public Thread{
private:
	HAL_I2C *i2c;
	uint64_t periode;
	HAL_GPIO imuGPIO;
	HAL_GPIO gyroGPIO;
	HAL_GPIO accGPIO;

	uint8_t readDataCmd[1];
	uint8_t magDataCmd[1];

	void initGyro();
	void readGyroData(uint8_t *buf, uint8_t bufferSize);
	bool writeReadCheck(uint8_t slave_address, uint8_t address, uint8_t cmd);
public:
	IMU(const char* name, HAL_I2C *i2c,  uint64_t periode);
	void init();
	void run();
};

#endif /* IMU_H_ */
