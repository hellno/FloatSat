/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

#define BUFFER_SIZE 128

Topic<CommStruct> tc(-1, "TC");
//CommBuffer<CommStruct> commBuffer;

CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) : Thread(name){
			this->periode = periode;
			this->uart = uart;
		}

		void CommHandler::init(){
			uart->init(115200);
		}

		void CommHandler::run(){

			TIME_LOOP(0, periode){

				if(uart->isDataReady()){
					char buf[BUFFER_SIZE];
					int size = uart->read(buf, BUFFER_SIZE);
					CommStruct cs;
					parseStringToPacket(buf, size, &cs);
					unsigned int retVal = tc.publish(cs);
					char out[BUFFER_SIZE];
					parsePacketToString(out, &cs);
					xprintf("%s\n",out);
				}
			}
	}

	void CommHandler::parseStringToPacket(char * str, int size, CommStruct* cs){
		if(size < 7)
			return;

		sprintf(cs->param, "%.6s", str);
		sprintf(cs->msg, "%.*s", size - 7, str + 6);
	}

	void CommHandler::parsePacketToString(char * out, CommStruct *cs){
		sprintf(out, "PARAM: '%s', MSG: '%s'", cs->param, cs->msg);
	}



