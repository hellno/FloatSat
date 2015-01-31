/*
 * Battery.h
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#ifndef HARDWARE_BATTERY_H_
#define HARDWARE_BATTERY_H_

#include "rodos.h"
#include "../topics.h"
#include "hal/hal_adc.h"

#define MAXVOLTAGE			12.0
#define MINVOLTAGE			9.0
#define MAXMINDIFF			MAXVOLTAGE - MINVOLTAGE

#define ADC_RESOLUTION		4095.0

class Battery : public Thread{
private:
	HAL_ADC *adc;
	uint64_t periode;
	bool active;
public:
	Battery(const char* name, uint64_t periode, HAL_ADC *adc);
	void init();
	void run();
	int32_t getBatteryRawValue(void);
	float getBatteryVoltage(void);
	float getBatteryPercent(void);

	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
};

#endif /* HARDWARE_BATTERY_H_ */
