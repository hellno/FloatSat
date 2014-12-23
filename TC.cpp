/*
 * TC.cpp
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#include "TC.h"
#include "stdlib.h"

static Fifo<CommStruct, 5> tcBuffer;
static bool buttonStatus;

TC::TC(const char* name, RODOS::TopicInterface & topic) : Subscriber(topic, name) {
	this->name = name;
	this->topicId = topic.topicId;
}

void TC::init(){
}


long TC::put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo) {
    xprintf("TID %ld PL %ld PD %ld\n", topicId, len, *(long*)data);
    switch(topicId){
    case UserButtonTopicID:
    	buttonStatus = data;
    	break;
    case TelecommandoTopicID:
    	 tcBuffer.put(*(CommStruct*)data);
    	break;
    }
    this->resume(); // not to publish from interrupt, call a thread to do it
    return 1;
 }

//seperate into two classes:
//one getting the TC::put requests
//-> which is then activating the threads, delivering the data, managing the state,...


void TC::run(){
	while(1){
		suspendCallerUntil();
		xprintf("resuming thread for topic: %d\n", this->topicId);
	    switch(this->topicId){
			case UserButtonTopicID:
				xprintf("sth happend with se button\n");
				//insert action for button change here
				if(buttonStatus)
					xprintf("button pressed!\n");
				else
					xprintf("button released!\n");
				break;
			case TelecommandoTopicID:
				CommStruct cs;
				tcBuffer.get(cs);

				handlePacket(&cs);
				break;
	    }

		}
}

void TC::handlePacket(CommStruct *cs){
	xprintf("handling se packet \n");
	xprintf("param: %s, msg: %s\n", cs->param, cs->msg);

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
