/*
 * AnglePID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "AnglePID.h"

Fifo<float, 5> angleFifo;
Subscriber angleSub(orientationTopic, angleFifo, "orientationSubForAnglePID");

AnglePID::AnglePID(void){
	P = 0.0;
	I = 0.0;
	D = 0.0;

	P_factor = -66.180;
	I_factor = -0.58;
	D_factor = 31.172;

	integral = 0.0;
	derivative = 0.0;
	prevError = 0.0;
	desAngle = 0.0;
	output = 0.0;

	tempVal = 0;
}

void AnglePID::run(void){
	angleFifo.get(tempVal);

	float error = tempVal - desAngle;

	if (error > 180) {
		error -= 360;
	} else if(error < -180){
		error += 360;
	}
	if (fabs(error) > PID_ERROR_THRESHOLD) {
		integral += error;
	}
	derivative = (error - prevError);

	P = P_factor * error;
	I = I_factor * integral;
	D = D_factor * derivative;

	output = P + I + D;

	prevError = error;

	if(DEBUG) xprintf("ANG_PID: %.2f (e:%.2f,desAng:%.2f,curAng:%.2f)\n", output, error, desAngle, tempVal);
}

float AnglePID::currentOutput(void){
	return output;
}

void AnglePID::setDestinationAngle(float angle){
	desAngle = angle;

	if(desAngle < 0){

		while(desAngle < 0){
			desAngle += 360;
		}

	} else if(desAngle > 360){

		while(desAngle > 360){
			desAngle -= 360;
		}

	}
	if(DEBUG) xprintf("A_PID new angle: %f\n", desAngle);
}

void AnglePID::setP(float p){
	this->P = p;
	if(DEBUG) xprintf("set AngleP const to %f\n", P);
}
void AnglePID::setI(float i){
	this->I = i;
	if(DEBUG) xprintf("set AngleI const to %f\n", I);
}
void AnglePID::setD(float d){
	this->D = d;
	if(DEBUG) xprintf("set AngleD const to %f\n", D);
}
float AnglePID::getP(void){
	return P;
}
float AnglePID::getI(void){
	return I;
}
float AnglePID::getD(void){
	return D;
}
