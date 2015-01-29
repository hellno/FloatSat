/*
 * RotPID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "RotPID.h"

Fifo<RawVector3D, 5> rotFifo;
Subscriber rotSub(gyroTopic, rotFifo, "velocitySubForRotPID");

RotPID::RotPID(void){
	P = 0.0;
	I = 0.0;
	D = 0.0;

	P_factor = -0.002932;
	I_factor = -0.586484;
	D_factor = 0;

	integral = 0.0;
	derivative = 0.0;
	prevError = 0.0;
	desRot = 0;
	output = 0.0;
}

void RotPID::run(void){
	rotFifo.get(tempVal);
	float error = tempVal.z - desRot;

	if (fabs(error) > PID_ERROR_THRESHOLD) {
		integral += error;
	}
	derivative = (error - prevError);

	P = P_factor * error;
	I = I_factor * integral;
	D = D_factor * derivative;

	output = P + I + D;

	prevError = error;

	if(DEBUG) xprintf("ROT_PID: %.2f (e:%.2f,desRot:%.2f,curRot:%d)\n", output, error, desRot, tempVal.z);
}

void RotPID::setDestinationRotation(int8_t rot){
	desRot = rot;

	if(DEBUG) xprintf("ROT_PID new rot: %f\n", desRot);
}

void RotPID::setP(float p){
	this->P = p;
	if(DEBUG) xprintf("set AngleP const to %f\n", P);
}

void RotPID::setI(float i){
	this->I = i;
	if(DEBUG) xprintf("set AngleI const to %f\n", I);
}

void RotPID::setD(float d){
	this->D = d;
	if(DEBUG) xprintf("set AngleD const to %f\n", D);
}

float RotPID::getP(void){
	return P;
}

float RotPID::getI(void){
	return I;
}

float RotPID::getD(void){
	return D;
}

uint16_t RotPID::currentOutput(void){
	return (uint16_t) output;
}
