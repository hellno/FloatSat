/*
 * AnglePID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "AnglePID.h"
#include "../Hardware/MotorThread.h"

Fifo<float, 2> angleFifo;
Subscriber angleSub(yawAngTopic, angleFifo, "orientationSubForAnglePID");

extern MotorThread mt;

AnglePID::AnglePID(void){
	P = 0.0;
	I = 0.0;
	D = 0.0;

	/* OLD VALUES */
	//P_factor = -66.180;
	//I_factor = -0.58;
	//D_factor = 31.172;

	/* 20 ms */
	//P_factor = 33.45;
	///I_factor = 0.8021;
	//D_factor = -3.815;
	//period = 0.01;

	/* 10 ms */
	P_factor = 12;
	I_factor = 0.002;
	D_factor =- 0.01;

	//PID Value Testing Andy
//	P_factor = -0.0306;
//	I_factor = 0;//.0000001;
//	D_factor = 0;//.005;
	//period = 0.01;

	integral = 0.0;
	derivative = 0.0;
	prevError = 0.0;
	desAngle = 0.0;
	output = 0.0;
	tempVal = 0;
	lastTime = 0;
}

void AnglePID::run(void){
	for(int i = 0; i < angleFifo.getElementCount(); i++) {
		angleFifo.get(tempVal);
	}

	float error = tempVal - desAngle;
	period = (NOW() - lastTime)/(MILLISECONDS*1000.0);
	xprintf("PERIOD: %f", period);
	if (error > 180) {
		error -= 360;
	} else if(error < -180){
		error += 360;
	}
	if (fabs(error) > PID_ERROR_THRESHOLD) {
		integral += error * period;
	}
	derivative = (error - prevError) / period;

	P = P_factor * error;
	I = I_factor * integral;
	D = D_factor * derivative;

	output = P + I + D;

	prevError = error;
	mt.setMotorSpeed(output);
	lastTime = NOW();
	xprintf("ANG_PID: %.2f (e:%.2f,desAng:%.2f,curAng:%.2f, p: %f)\n", output, error, desAngle, tempVal, period);
}

int16_t AnglePID::currentOutput(void){
	return (int16_t) output;
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
	xprintf("A_PID new angle: %f(param=%f)\n", desAngle, angle);
}

void AnglePID::setPeriod(float seconds){
	this->period = seconds;
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

float AnglePID::getError(void){
	return prevError;
}

float AnglePID::getOutput(void){
	return output;
}

