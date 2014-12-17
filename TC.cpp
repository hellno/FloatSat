/*
 * TC.cpp
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#include "TC.h"
#include "stdlib.h"

static SyncFifo<CommStruct, 5> tcBuffer;
static Subscriber tcSubscriber(tc, tcBuffer, "TC");

TC::TC(const char* name){

}

void TC::init(){

}

void TC::run(){
	while(1){
			CommStruct cs;
			tcBuffer.syncGet(cs);

			handlePacket(&cs);
		}
}

void TC::handlePacket(CommStruct *cs){
//	xprintf("handling se packet: \n");
//	xprintf("param: %s, msg: %s\n", cs->param, cs->msg);

	if (paramIsEqualTo(cs, "SETPWM")){
			uint8_t nr = atoi(cs->msg);
			motorSpeedTopic.publish(nr);
	} else if (paramIsEqualTo(cs, "SETYAW")) {

	} else if (paramIsEqualTo(cs, "SETROT")) {

	} else {
		xprintf("unknown tc packet received\n");
	}
}
bool TC::paramIsEqualTo(CommStruct *cs, const char* param){
	return strcmp(cs->param, param) == 0;
}

void TC::changeMode(SkyNetModes mode){

}
