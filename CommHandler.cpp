/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

#include <string>
#include <stdlib.h>

#define BUFFER_SIZE 256

Topic<CommStruct> tcTopic(-1, "TC");
Topic<CommStruct> tmTopic(-1, "TM");
Fifo<CommStruct, 64> tmFifo;
Subscriber tmSubscriber(tmTopic, tmFifo, "tmSub");

CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) :
		Thread(name) {
	this->periode = periode;
	this->uart = uart;
}

void CommHandler::init() {
	uart->init(115200);
}

void CommHandler::run() {
	char buf[BUFFER_SIZE];
	char out[BUFFER_SIZE];
	int size = 0;
	CommStruct cs;

	TIME_LOOP(0, periode)
	{
		if (uart->isDataReady()) {

			size = uart->read(buf, BUFFER_SIZE);

			if (parseStringToPacket(buf, size, &cs)) {
				tcTopic.publish(cs);
			}

//			ECHO
//			parsePacketToString(out, &cs);
//			xprintf("ECHO %s\n", out);
		}

		if (!tmFifo.isEmpty()) {
			tmFifo.get(cs);
		}
		cs = CommStruct();
	}
}

bool CommHandler::parseStringToPacket(char * str, int size, CommStruct* cs) {
	if (size < 7 && strlen(str) < 7)
		return false;

//	xprintf("msg-size: %d, strlen: %d\n", size - 7, strlen(str));

	sprintf(cs->param, "%.6s", str);

	char * tmpVal;
	tmpVal = (char*) malloc (size - 6);
	tmpVal[size - 7] = '\0';

	uint8_t j = 0;
	for(uint8_t i = 6; i < strlen(str); i++){
		*(tmpVal + j) = *(str+i);
		j++;
	}

	cs->msg = tmpVal;
	return true;
}

void CommHandler::parsePacketToString(char * out, CommStruct *cs) {
	sprintf(out, "PARAM: '%s', MSG: '%s'", cs->param, cs->msg);
}

