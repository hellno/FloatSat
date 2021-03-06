/*
 * RotPID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "RotPID.h"
#include "../Hardware/MotorThread.h"

Fifo<Vector3D, 5> rotFifo;
Subscriber rotSub(gyroTopic, rotFifo, "velocitySubForRotPID");

extern MotorThread mt;

RotPID::RotPID(void){
	P = 0.0;
	I = 0.0;
	D = 0.0;

	/* OLD VALUES
	P_factor = -0.002932;
	I_factor = -0.586484;
	D_factor = 0;
	*/

	/* 20 MS */
	P_factor = -4.86;
	I_factor = -81.70;
	D_factor = 0.33791;


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
		integral += error * period;
	}
	derivative = (error - prevError) / period;

	P = P_factor * error;
	I = I_factor * integral;
	D = D_factor * derivative;

	output = P + I + D;

	prevError = error;
	mt.setMotorSpeed(output);

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

int16_t RotPID::currentOutput(void){
	return (uint16_t) output;
}

void RotPID::setPeriod(float seconds){
	this->period = seconds;
}

float RotPID::getOutput(void){
	return output;
}

float RotPID::getError(void){
	return prevError;
}
