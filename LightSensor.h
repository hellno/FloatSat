/*
 * LightSensor.h
 *
 *  Created on: 28.11.2014
 *      Author: holger
 */

#ifndef LIGHTSENSOR_H_
#define LIGHTSENSOR_H_

#include "rodos.h"
#include "topics.h"
#include "hal/hal_i2c.h"

/*
 * Light Sensor - TSL2561
 * Low Gain (1x), integration time of 101ms
 * PINS PINS PINS:
 * I2C_IDX1:
 * 	SCL PB6/PB8
 *	SDA PB7/PB9
 * I2C_IDX2:
 * 	SCL PB10/PF1/PH4
 *	SDA PB11/PF0/PH5
 * I2C_IDX3: <---
 * 	SCL PA8/PH7
 *	SDA PC9/PH8

 *
 * Slave Address:
 * GND = 0101001 <--
 * FLOAT = 0111001
 * VCC = 1001001
 */

class LightSensor  : public Thread{
private:
HAL_I2C *i2c;
uint64_t periode;
public:
	LightSensor(const char* name, HAL_I2C *i2c,  uint64_t periode);
	void init();
	void run();
	uint32_t calculateLux(uint16_t ch0, uint16_t ch1);
	void turnOn(void);
	void turnOff(void);
	void setPeriode(uint64_t periode);
};


#endif /* LIGHTSENSOR_H_ */
