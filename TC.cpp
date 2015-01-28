/*
 * TC.cpp
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#include "TC.h"
#include "stdlib.h"


Fifo<CommStruct, 5> tcBuffer;

bool DEBUG = true;
bool DBGOUT = false;

extern TM tm;
extern Satellite skyNet;

TC::TC(const char* name, IMU *imu, LightSensor *ls, MotorThread *mt) : Subscriber(tcTopic, name){
	this->imu = imu;
	this->ls = ls;
	this->mt = mt;
}

long TC::put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
    tcBuffer.put(*(CommStruct*)data);
    this->resume();
    return 1;
 }
void TC::init(void){

}

void TC::run(){
	CommStruct cs;
	while(1){
		suspendCallerUntil();
		tcBuffer.get(cs);
		handlePacket(&cs);
		cs = CommStruct();
	}
}

void TC::handlePacket(CommStruct *cs){
	if (DEBUG) xprintf("TC: param:%s,msg:%s\n", cs->param, cs->msg);

	if (paramIsEqualTo(cs, "MOTSPD")){ // [0,100] set % of motorspeed
			mt->setMotorSpeed(atoi(cs->msg));
	} else if (paramIsEqualTo(cs, "SETYAW")) { // [0,360] set yaw angle
		skyNet.setDestinationAngle(atoi(cs->msg));
	} else if (paramIsEqualTo(cs, "SETROT")) { // TODO! - set rotation speed
		xprintf("TODO SETROT");
		skyNet.setDestinationRotation(atoi(cs->msg));
	} else if(paramIsEqualTo(cs, "IMUPRD")){ // [ms] change the IMU refresh period
		imu->setPeriode(atoi(cs->msg) * MILLISECONDS);
	} else if(paramIsEqualTo(cs, "LGTPRD")){ // [ms] change the LightSensor period
		ls->setPeriode(atoi(cs->msg) * MILLISECONDS);
	}else if(paramIsEqualTo(cs, "IMUINI")){ // restart the IMU
		imu->init();
	} else if(paramIsEqualTo(cs, "IMUCAL")){ //recalibrate the IMU
		imu->calcBias();
	} else if(paramIsEqualTo(cs, "MOTINI")){ // restart the Motor
		mt->init();
	} else if(paramIsEqualTo(cs, "LGTSTP")){ //stop the LightSensor
		ls->turnOff();
	} else if(paramIsEqualTo(cs, "LGTRUN")){ //start the LightSensor
		ls->turnOn();
	} else if(paramIsEqualTo(cs, "TMSTOP")){ //stop the telemetry
		tm.turnOff();
	}else if(paramIsEqualTo(cs, "TMSTRT")){ // start the telemetry
		tm.turnOn();
	/* DEBUG OUTPUT FLAGS*/
	}else if (paramIsEqualTo(cs, "DBGOUT")){ // 1 -> DBGOUT on, else -> off
		if(msgIsEqualTo(cs, "1")){
			DBGOUT = true;
		}else{
			DBGOUT = false;
		}
		if (DEBUG) xprintf("DBGOUT is %d\n", DBGOUT);
	} else if(paramIsEqualTo(cs, "DEBUGG")){ // 1 -> DEBUG on, else -> off
		if(msgIsEqualTo(cs, "1")){
			DEBUG = true;
		}else{
			DEBUG = false;
		}
	/* SkyNet Modes */
	} else if(paramIsEqualTo(cs, "STDNBY")){ // standby mode
		skyNet.setMode(STDNBY);
	} else if(paramIsEqualTo(cs, "ROTMOD")){ // rotation mode
		skyNet.setMode(ROTMOD);
	} else if(paramIsEqualTo(cs, "COMPAS")){ // compass/pointing mode
		skyNet.setMode(COMPAS);
	} else if(paramIsEqualTo(cs, "SUNFIN")){ // sunfinding mode
		skyNet.setMode(SUNFIN);
	} else if(paramIsEqualTo(cs, "MISION")){ // mission mode
		skyNet.setMode(MISION);
	/* Angle PID constants */
	} else if(paramIsEqualTo(cs, "ANGCSP")){ // P const of angle PID controller
		skyNet.setAnglePIDConst(P, atof(cs->msg));
	} else if(paramIsEqualTo(cs, "ANGCSI")){ // I const of angle PID controller
		skyNet.setAnglePIDConst(I, atof(cs->msg));
	} else if(paramIsEqualTo(cs, "ANGCSD")){ // D const of angle PID controller
		skyNet.setAnglePIDConst(D, atof(cs->msg));
	} else {
		if (DEBUG) xprintf("unknown tc packet received\n");
	}
}

bool TC::paramIsEqualTo(CommStruct *cs, const char* param){
	return strcmp(cs->param, param) == 0;
}

bool TC::msgIsEqualTo(CommStruct *cs, const char* msg){
	return strcmp(cs->msg, msg) == 0;
}

