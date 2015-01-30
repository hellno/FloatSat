/*
 * TC.cpp
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#include "TC.h"
#include "stdlib.h"


Fifo<CommStruct, 25> tcBuffer;

bool DEBUG = true;
bool DBGOUT = false;

extern TM tm;
extern Satellite skyNet;

TC::TC(const char* name, IMU *imu, LightSensor *ls, MotorThread *mt, Camera *camera) : Subscriber(tcTopic, name){
	this->imu = imu;
	this->ls = ls;
	this->mt = mt;
	this->camera = camera;
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
	if(DEBUG){
		xprintf("TC: param:%s,msg:%s(dbg=%d)\n", cs->param, cs->msg, DEBUG);
		xprintf("len: p%d,m%d\n", strlen(cs->param), strlen(cs->msg));
	}

	if (paramIsEqualTo(cs, "MOTSPD")){ // [0,100] set % of motorspeed
			mt->setMotorSpeed(atoi(cs->msg));
	} else if (paramIsEqualTo(cs, "ECHOHO")) { // echo test
		xprintf("ECHOHO(DBG)\n");
	} else if (paramIsEqualTo(cs, "SETYAW")) { // [0,360] set yaw angle
		skyNet.setDestinationAngle(atoi(cs->msg));
	} else if (paramIsEqualTo(cs, "SETROT")) { // TODO! - set rotation speed
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
	/* Rotation PID constants */
	} else if(paramIsEqualTo(cs, "ROTCSP")){ // P const of rot PID controller
		skyNet.setRotPIDConst(P, atof(cs->msg));
	} else if(paramIsEqualTo(cs, "ROTCSI")){ // I const of rot PID controller
		skyNet.setRotPIDConst(I, atof(cs->msg));
	} else if(paramIsEqualTo(cs, "ROTCSD")){ // D const of rot PID controller
		skyNet.setRotPIDConst(D, atof(cs->msg));
	} else if(paramIsEqualTo(cs, "CAPPIC")){ // CAM1: capture picture
		skyNet.capturePicture();
	} else if(paramIsEqualTo(cs, "DETECT")){ // CAM2: detect target
			skyNet.camDetect();
	} else if(paramIsEqualTo(cs, "SNDPIC")){ // CAM3: send picture
		skyNet.sendPicture();
	} else if(paramIsEqualTo(cs, "CALIBM")){ //[0,1] de/activate output of mag. calib values
		if(msgIsEqualTo(cs, "1")){
			imu->setMagCalibMode(true);
		}else{
			imu->setMagCalibMode(false);
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

