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
#include "gyro.h"
#include "acc.h"

extern HAL_I2C imuI2C;

class IMU  : public Thread{
private:

	HAL_GPIO imuGPIO;
	HAL_GPIO accGPIO;

	Gyro gyro;
	Acc acc;

	void calcGyroBias(void);
	void calcAccBias(void);
	void writeDataToVector(float *data, Vector3D *vector);
	bool writeReadCheck(uint8_t slave_address, uint8_t address, uint8_t cmd);
	bool readCheck(uint8_t slave_address, uint8_t address, uint8_t targetValue);
	void mergeRawData(uint8_t *from, RawVector3D to);
public:
	IMU(const char* name);
	void init();
	void run();
};

#endif /* IMU_H_ */
