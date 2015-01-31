/*
 * SolarPanel.cpp
 *
 *  Created on: 31.01.2015
 *      Author: holger
 */

#include "SolarPanel.h"

Topic<float> solarpanelChargeTopic(-1, "SolarPanelChargeCurrentTopic");

SolarPanel::SolarPanel(const char* name, uint64_t periode, HAL_ADC *adc1, HAL_ADC *adc2) : Thread(name){
	this->periode = periode;
	this->active = true;

	this->adc1 = adc1;
	this->adc2 = adc2;
}

void SolarPanel::init(){
	this->adc1->init(SOLAR_PANEL1_CHANNEL);
	this->adc2->init(SOLAR_PANEL2_CHANNEL);
}

void SolarPanel::run(){
	float tempCharge;

	while(1){
		if(active){
			suspendCallerUntil(NOW() + periode);

			tempCharge = getCurrent();

			solarpanelChargeTopic.publish(tempCharge);
		}
	}
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

int32_t SolarPanel::getRawVoltage1(void){
	return adc1->read(ADC_CH_001);
}

int32_t SolarPanel::getRawVoltage2(void){
	return adc2->read(ADC_CH_002);
}

float SolarPanel::getCurrent(void){
	return (getRawVoltage1() - getRawVoltage2()) / SOLAR_PANEL_RESISTOR;
}
