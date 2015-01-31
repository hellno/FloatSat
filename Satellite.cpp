/*
 * Satellite.cpp
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#include "Satellite.h"
#include "TC.h"
#include "TM.h"
#include "Camera/Camera.h"
#include "Hardware/Motor.h"
#include "CommHandler.h"

extern TC tc;
extern TM tm;
extern CommHandler ch;
extern Camera camera;
extern Motor mt;

Satellite::Satellite(const char* name, uint64_t periode) : Thread(name),
		firePWM(PWM_IDX00),pwmGPIO(GPIO_073){
	this->periode = periode;
	pwmGPIO.init(false,1,0);
	firePWM.init(50,100);
	firePWM.write(4);
	mode = STDNBY;

	this->anglePID = AnglePID();
	this->rotPID = RotPID();

	anglePID.setPeriod(periode / 1000);
	rotPID.setPeriod(periode / 1000);
}

void Satellite::init(void){

}

void Satellite::setDestinationAngle(uint8_t angle){
	if(mode == COMPAS){
		anglePID.setDestinationAngle(angle);
	}
}

void Satellite::setDestinationRotation(int8_t rot){
	if(mode == ROTMOD){
		rotPID.setDestinationRotation(rot);
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
		break;
	case ROTMOD:
		rotPID.run();
		tempValue = rotPID.currentOutput();
		motorSpeedTopic.publish(tempValue);
		break;
	case COMPAS:
		anglePID.run();
		tempValue = anglePID.currentOutput();
		motorSpeedTopic.publish(tempValue);
		break;
	case MISION:

		break;
	}
}

void Satellite::setPeriode(uint64_t periode){
	this->periode = periode;
	anglePID.setPeriod(periode / 1000);
	rotPID.setPeriod(periode / 1000);

	if (DEBUG) xprintf("changed satellite period to: %ld\n", this->periode);
}

void Satellite::setMode(SkyNetMode newMode){

	if(newMode == mode){
		return;
	}

	mode = newMode;
	switchMode();

}
/* Camera mission mode */
void Satellite::sendPicture(void){
	if(mode == MISION){
		camera.sendPicture(ch.getUart());
	}
}

/*void Satellite::camDetect(void){
	if(mode == MISION){
		camera.DetectSatellite();
	}
}

void Satellite::capturePicture(void){
	if(mode == MISION){
		camera.Capture();
	}
}*/


void Satellite::switchMode(void){
	switch(mode){
	case STDNBY:
		camera.turnOff();
		break;
	case ROTMOD:

		break;
	case COMPAS:

		break;
	case SUNFIN:

		break;
	case MISION:
		camera.turnOn();
		break;
	}
	xprintf("switched to mode %d\n", mode);
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

void Satellite::fireNet() {
	if(mode == MISION){
		firePWM.write(11);
	}
}

void Satellite::setRotPIDConst(PIDConstant select, float val){
	switch(select){
	case(P):
		rotPID.setP(val);
		break;
	case(I):
		rotPID.setI(val);
		break;
	case(D):
		rotPID.setD(val);
		break;
	default:
		return;
	}
}
