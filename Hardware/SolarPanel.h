/*
 * SolarPanel.h
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#ifndef CAMERA_SOLARPANEL_H_
#define CAMERA_SOLARPANEL_H_

#include "rodos.h"
#include "../topics.h"
#include "hal/hal_adc.h"

class SolarPanel : public Thread{
private:
	HAL_ADC *adc1;
	HAL_ADC *adc2;

	uint64_t periode;
	bool active;
public:
	SolarPanel(const char* name, uint64_t periode, HAL_ADC *adc1, HAL_ADC *adc2);
	void init();
	void run();

	int32_t getRawVoltage1(void);
	int32_t getRawVoltage2(void);
	float getCurrent(void);

	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
};

#endif /* CAMERA_SOLARPANEL_H_ */
