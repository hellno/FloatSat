/*
 * MotorThread.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "MotorThread.h"
#include "topics.h"
#include "fifo.h"

MotorThread::MotorThread(const char* name) : Thread(name){
}

void MotorThread::init(){
	motor.init();

	//	motor.startMotor();
//	suspendCallerUntil(NOW() + 10 * SECONDS);
//	motor.stopMotor();

	if (DEBUG) xprintf("mt init done\n");
}

void MotorThread::run(){
	uint8_t motorSpeed;

	while(1){
		suspendCallerUntil();
	}
}

void MotorThread::setMotorSpeed(uint8_t newMotorSpeed){
	motor.setSpeed(newMotorSpeed);
}
