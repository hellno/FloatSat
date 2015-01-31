/*
 * Battery.cpp
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#include "Battery.h"

Topic<float> batteryPercentageTopic(-1, "BatteryPercentageTopic");
Topic<float> batteryVoltageTopic(-1, "BatteryVoltageTopic");

Battery::Battery(const char* name, uint64_t periode, HAL_ADC *adc) : Thread(name){
		this->periode = periode;
		this->active = true;
		this->adc = adc;
}

void Battery::init(void){
	adc->init(BATTERY_CHANNEL); // PA0/PA0/PA0
}

void Battery::run(void){
	float tempPercentage, tempVoltage;

	while(1){
		if(active){
			suspendCallerUntil(NOW() + periode);

			tempPercentage = getBatteryPercent();
			tempVoltage = getBatteryVoltage();

			batteryPercentageTopic.publish(tempPercentage);
			batteryVoltageTopic.publish(tempVoltage);
		}
	}
}

int32_t Battery::getBatteryRawValue(void){
	return adc->read(BATTERY_CHANNEL);
}

float Battery::getBatteryPercent(void){
	return (getBatteryVoltage() - MINVOLTAGE) / MAXMINDIFF * 100;
}

float Battery::getBatteryVoltage(void){
	return getBatteryRawValue()/ADC_RESOLUTION * MAXVOLTAGE;
}

void Battery::turnOn(void){
	active = true;
}
void Battery::turnOff(void){
	active = false;
}
bool Battery::isOn(void){
	return active;
}
