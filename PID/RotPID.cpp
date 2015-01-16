/*
 * RotPID.cpp
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#include "RotPID.h"



RotPID::RotPID(void){
	P = 0.0;
	I = 0.0;
	D = 0.0;

	prevError = 0.0;
	desRot = 0;
}

void RotPID::run(void){
}
