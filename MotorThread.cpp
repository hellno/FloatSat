/*
 * MotorThread.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "MotorThread.h"

MotorThread::MotorThread(const char* name) : Thread(name){
}

void MotorThread::init(){
	motor.init();
}

void MotorThread::run(){

}
