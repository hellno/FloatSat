/*
 * AnglePID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "AnglePID.h"

Fifo<uint8_t, 5> angleFifo;
Subscriber angleSub(orientationTopic, angleFifo, "orientationSubForAnglePID");

uint8_t tempVal;

AnglePID::AnglePID(void){
	if (DEBUG) xprintf("anglePID activated");

	P = 0.0;
	I = 0.0;
	D = 0.0;

	prevError = 0.0;
	desAngle = 0;
}

void AnglePID::run(void){
	angleFifo.get(tempVal);

	float error = tempVal - desAngle;

	if (error > 180) {
		error -= 360;
	} else if(error < -180){
		error += 360;
	}
}

void AnglePID::setDestinationAngle(uint8_t angle){
	desAngle = angle;

	if(angle < 0){
		while(desAngle < 0){
			desAngle += 360;
		}
	} else if(angle > 360){
		while(desAngle > 360){
			desAngle -= 360;
		}
	}

}
