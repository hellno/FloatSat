/*
 * Battery.cpp
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#include "SolarPanel.h"

Topic<float> batteryVoltageTopic(-1, "BatteryVoltageTopic");

SolarPanel::SolarPanel(const char* name, uint64_t periode, HAL_ADC *adc) : Thread(name){
	this->periode = periode;
	this->active = true;
	this->adc = adc;
}

void SolarPanel::init(void){
	adc->init(BATTERY_CHANNEL);
}

void SolarPanel::run(void){
	float tempPercentage, tempVoltage;

	while(1){
		if(active){
			suspendCallerUntil(NOW() + periode);

			tempVoltage = getSolarPanelVoltage();

			batteryVoltageTopic.publish(tempVoltage);
		}
	}
}

int32_t SolarPanel::getSolarPanelRawValue(void){
	return adc->read(BATTERY_CHANNEL);
}

float SolarPanel::getSolarPanelPercent(void){
	return (getSolarPanelVoltage() - MINVOLTAGE) / MAXMINDIFF * 100;
}

float SolarPanel::getSolarPanelVoltage(void){
	return getSolarPanelRawValue()/ADC_RESOLUTION * MAXVOLTAGE * SOLARPANEL_FACTOR;
}

void SolarPanel::turnOn(void){
	active = true;
}
void SolarPanel::turnOff(void){
	active = false;
}
bool SolarPanel::isOn(void){
	return active;
}
