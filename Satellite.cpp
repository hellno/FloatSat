/*
 * Satellite.cpp
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#include "Satellite.h"
#include "TC.h"
#include "TM.h"

extern TC tc;
extern TM tm;

Satellite::Satellite(const char* name, uint64_t periode) : Thread(name){
	if(periode < 100 * MILLISECONDS)
		this->periode = 100 * MILLISECONDS;
	else
		this->periode = periode;

	mode = STDNBY;

	this->anglePID = AnglePID();
	this->rotPID = RotPID();
}

void Satellite::init(void){

}

void Satellite::setDestinationAngle(uint8_t angle){
	if(mode == COMPAS){
		anglePID.setDestinationAngle(angle);
	}
}

void Satellite::run(void){
	while(1){
		suspendCallerUntil(NOW() + periode);

		handleModePeriodic();
	}
}

void Satellite::handleModePeriodic(void){
	switch(mode){
	case STDNBY:
		return;
	case ROTMOD:
		rotPID.run();
		break;
	case COMPAS:
		anglePID.run();
		break;
	}
}

void Satellite::setPeriode(uint64_t periode){
	this->periode = periode;
	if (DEBUG) xprintf("changed satellite period to: %ld\n", this->periode);
}

void Satellite::setMode(SkyNetMode newMode){
	if(newMode == mode){
		return;
	}

	mode = newMode;
	switchMode();

}
void Satellite::switchMode(void){
	switch(mode){
	case STDNBY:

		break;
	case ROTMOD:

		break;
	case COMPAS:

		break;
	case SUNFIN:

		break;
	case MISION:

		break;
	}
	if(DEBUG) xprintf("switched to mode %d\n", mode);

}
SkyNetMode Satellite::getCurrentMode(void){
	return mode;
}

void Satellite::setAnglePIDConst(PIDConstant select, float val){
	switch(select){
	case(P):
			anglePID.setP(val);
			break;
	case(I):
			anglePID.setI(val);
			break;
	case(D):
			anglePID.setD(val);
			break;
	default:
		return;
	}
}
