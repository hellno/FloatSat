/*
 * MotorThread.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "MotorThread.h"
#include "fifo.h"

Topic<uint16_t> motorSpeedTopic(-1, "motorSpeedTopic");


SyncFifo<uint16_t, 10> fifo;
Subscriber nameNotImportant02(motorSpeedTopic, fifo, "motorSpeedSubscriber");

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
	uint16_t motorSpeed;

	while(1){
		suspendCallerUntil();
		fifo.syncGet(motorSpeed);
		this->setMotorSpeed(motorSpeed);
	}
}

void MotorThread::setMotorSpeed(uint16_t newMotorSpeed){
	motor.setSpeed(newMotorSpeed);
}
