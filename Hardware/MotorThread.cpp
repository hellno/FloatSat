/*
 * MotorThread.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "MotorThread.h"
#include "fifo.h"

Topic<int16_t> motorSpeedTopic(-1, "motorSpeedTopic");

Fifo<int16_t, 2> fifo;
Subscriber nameNotImportant02(motorSpeedTopic, fifo, "motorSpeedSubscriber");
int16_t tmp;

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
	int16_t motorSpeed;
//
//	while(1){
//		suspendCallerUntil(NOW() + 20*MILLISECONDS);
//
//		if(fifo.get(motorSpeed)){
//			this->setMotorSpeed(motorSpeed);
//		}
//	}
}

void MotorThread::setMotorSpeed(int16_t newMotorSpeed){
	motor.setSpeed(newMotorSpeed);
	tmp = motor.getSpeed();

	motorSpeedTopic.publish(tmp);
	xprintf("new motspd: %d\n", tmp);
}

int16_t MotorThread::getMotorSpeed(void){
	return motor.getSpeed();
}
