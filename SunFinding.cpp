/*
 * SunFinding.cpp
 *
 *  Created on: 01.02.2015
 *      Author: holger
 */

#include "SunFinding.h"

#define SF_FIFO_SIZE 10

Fifo<uint32_t, SF_FIFO_SIZE> luxSFFifo;
Fifo<float, 2> angleSFFifo;

Subscriber luxSFSubscriber(lightTopic, luxSFFifo, "lightSub for SunFinding");
Subscriber yawAngleSFSubscriber(yawAngTopic, angleSFFifo, "yawAngleSub for SunFinding");

SunFinding::SunFinding(const char* name) : Thread(name){
	this->maxLux = 0;
	this->active = false;
	this->angleAtMaxLux = 0;

	this->sunFound = false;
}

void SunFinding::init(void){

}

void SunFinding::run(void){
	uint32_t avgLux;
	while(1){
		suspendCallerUntil(NOW() + 250 * MILLISECONDS);
		if(active){
			avgLux = getAvgLux();
			if(avgLux > maxLux){
				maxLux = avgLux;
				angleSFFifo.get(angleAtMaxLux);
			}
		}
	}
}

uint32_t SunFinding::getMaxLux(void){
	return this->maxLux;
}

float SunFinding::getSunAngle(void){
	return this->angleAtMaxLux;
}

uint32_t SunFinding::getAvgLux(void){
	uint64_t luxSum = 0;
	uint32_t tempVal;
	uint8_t cnt = 0;
	while(!luxSFFifo.isEmpty() && cnt < SF_FIFO_SIZE){
		luxSFFifo.get(tempVal);
		luxSum += tempVal;
		cnt++;
	}
	luxSum /= cnt;

	xprintf("avg lux: %d (cnt=%d)\n", luxSum, cnt);

	return luxSum;
}

void SunFinding::turnOn(void){
	active = true;
}
void SunFinding::turnOff(void){
	active = false;
}
bool SunFinding::isOn(void){
	return active;
}

