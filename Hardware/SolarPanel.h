/*
 * Battery.h
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#ifndef HARDWARE_SOLARPANEL_H_
#define HARDWARE_SOLARPANEL_H_

#include "rodos.h"
#include "../topics.h"
#include "hal/hal_adc.h"

#define MAXVOLTAGE			3.3f
#define MINVOLTAGE			0.0f
#define MAXMINDIFF			MAXVOLTAGE - MINVOLTAGE
#define SOLARPANEL_FACTOR	3.7f
#define ADC_RESOLUTION		4095.0

class SolarPanel : public Thread{
private:
	HAL_ADC *adc;
	uint64_t periode;
	bool active;
public:
	SolarPanel(const char* name, uint64_t periode, HAL_ADC *adc);
	void init();
	void run();
	int32_t getSolarPanelRawValue(void);
	float getSolarPanelVoltage(void);
	float getSolarPanelPercent(void);

	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
};

#endif /* HARDWARE_SOLARPANEL_H_ */
