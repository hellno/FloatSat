/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

#define BUFFER_SIZE 256

Topic<CommStruct> tc(-1, "TC");
char* sendString;

CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) : Thread(name){
			this->periode = periode;
			this->uart = uart;
		}

		void CommHandler::init(){
			uart->init(115200);
		}

		void CommHandler::run(){
			char buf[BUFFER_SIZE];
			char out[BUFFER_SIZE];
			int size = 0;
			CommStruct cs;

			TIME_LOOP(0, periode){

				if(uart->isDataReady()){

					size = uart->read(buf, BUFFER_SIZE);

					parseStringToPacket(buf, size, &cs);

					tc.publish(cs);

//					ECHO
//					parsePacketToString(out, &cs);
//					xprintf("%s\n",out);
				}
			}
	}

	void CommHandler::parseStringToPacket(char * str, int size, CommStruct* cs){
		if(size < 7)
			return;

		xprintf("length: %d\n", size);

		sprintf(cs->param, "%.6s", str);
		strncpy(cs->msg, str + 6, size - 6);
		//sprintf(cs->msg, "%.*s", str - 7, str + 6);
	}

	void CommHandler::parsePacketToString(char * out, CommStruct *cs){
		sprintf(out, "PARAM: '%s', MSG: '%s'", cs->param, cs->msg);
	}

	void CommHandler::sendPacket(SkyNetTMType paramType, char* msg){
		switch(paramType){
		case 1:
			break;
		default:
			xprintf("wrong TM type\n");
			break;
		}
	}



