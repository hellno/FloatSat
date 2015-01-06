/*
 * MotorThread.h
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#ifndef MOTORTHREAD_H_
#define MOTORTHREAD_H_

#include "rodos.h"
#include "Motor.h"

class MotorThread : public Thread {
private:
	Motor motor;
public:
	MotorThread(const char* name);
	void init();
	void run();
	void setMotorSpeed(uint8_t newMotorSpeed);
};

#endif /* MOTORTHREAD_H_ */
