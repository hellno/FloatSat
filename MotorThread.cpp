/*
 * MotorThread.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "MotorThread.h"
#include "topics.h"
#include "fifo.h"

Topic<uint8_t> motorSpeedTopic(-1, "motorSpeedTopic");

static SyncFifo<uint8_t, 5> motorSpeedBuffer;
static Subscriber motorSubscriber(motorSpeedTopic, motorSpeedBuffer, "motorSpeed");

MotorThread::MotorThread(const char* name) : Thread(name){
}

void MotorThread::init(){
	motor.init();
	motor.startMotor();
	suspendCallerUntil(NOW() + 10 * SECONDS);
	motor.stopMotor();
	xprintf("stopped the motor");
}

void MotorThread::run(){
	uint8_t motorSpeed;

	while(1){
		motorSpeedBuffer.syncGet(motorSpeed);
		xprintf("new motor speed received: %d", motorSpeed);
		motor.setSpeed(motorSpeed);
	}
}
