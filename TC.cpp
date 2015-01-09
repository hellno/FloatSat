/*
 * TC.cpp
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#include "TC.h"
#include "stdlib.h"
#include "topics.h"

static Fifo<CommStruct, 5> tcBuffer;
bool DEBUG = true;
bool DBGOUT = false;

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
	if (DEBUG) xprintf("rec: param:%s,msg:%s\n", cs->param, cs->msg);

	if (paramIsEqualTo(cs, "SETPWM")){
			mt->setMotorSpeed(atoi(cs->msg));
	} else if (paramIsEqualTo(cs, "SETYAW")) {
		xprintf("TODO SETROT");
	} else if (paramIsEqualTo(cs, "SETROT")) {
		xprintf("TODO SETROT");
	} else if(paramIsEqualTo(cs, "IMUPRD")){
		imu->setPeriode(atoi(cs->msg) * MILLISECONDS);
	} else if(paramIsEqualTo(cs, "LGTPRD")){
		ls->setPeriode(atoi(cs->msg) * MILLISECONDS);
	}else if(paramIsEqualTo(cs, "IMUINI")){
		imu->init();
	} else if(paramIsEqualTo(cs, "IMUCAL")){
		imu->calcBias();
	} else if(paramIsEqualTo(cs, "MOTINI")){
		mt->init();
	} else if(paramIsEqualTo(cs, "LGTSTP")){
		ls->suspendCallerUntil();
		xprintf("stopped lightSensor\n");
	} else if(paramIsEqualTo(cs, "LGTRUN")){
		ls->resume();
		xprintf("started lightSensor\n");
	}else if (paramIsEqualTo(cs, "DBGOUT")){
		if(msgIsEqualTo(cs, "1")){
			DBGOUT = true;
		}else{
			DBGOUT = false;
		}
	} else if(paramIsEqualTo(cs, "DEBUGG")){
		if(msgIsEqualTo(cs, "1")){
			DEBUG = true;
		}else{
			DEBUG = false;
		}
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
void TC::changeMode(SkyNetModes mode){

}
