/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

#include <string>
#include <stdlib.h>

Topic<CommStruct> tcTopic(-1, "TC");
Topic<CommStruct> tmTopic(-1, "TM");
Fifo<CommStruct, 64> tmFifo;
Subscriber tmSubscriber(tmTopic, tmFifo, "tmSub");

char strBuf[BUFFER_SIZE];

CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) :
		Thread(name) {
	this->periode = periode;
	this->uart = uart;
}

void CommHandler::init(void) {
	uart->init(115200);
}

void CommHandler::run(void) {
	char buf[BUFFER_SIZE];

	int size = 0;
	CommStruct cs;

	TIME_LOOP(0, periode)
	{
		if (uart->isDataReady()) {

			size = uart->read(buf, BUFFER_SIZE);

			if (parseStringToPacket(buf, size, &cs)) {
				tcTopic.publish(cs);
			}

/*		ECHO	*/
//			xprintf("--> ECHO p:%.6s m:%s\n", cs.param, cs.msg);
		}

		if(tmFifo.get(cs)) {
			parsePacketToString(buf, &cs);
//			xprintf("buf: %s\n", buf);
//			xprintf("len: param: %d, msg: %d\n", strlen(cs.param), strlen(cs.msg));
			uart->write(buf, 8 + strlen(cs.msg));
		}
		cs = CommStruct();
	}
}

bool CommHandler::parseStringToPacket(char * str, int size, CommStruct* cs) {
	if (size < 7 && strlen(str) < 7)
		return false;

	size_t len = strlen(str) - 6;

//	xprintf("msg-len: %d\n", len);

	sprintf(cs->param, "%.6s\0", str);

	if(len > 0){
		char tmpVal[len];

		for(uint8_t i = 0; i < len; i++){
			*(tmpVal + i) = *(str + 6 + i);
		}

		sprintf(cs->msg, "%s\0", tmpVal);
	}

	return true;
}

void CommHandler::parsePacketToString(char * outStr, CommStruct *cs) {
	//sprintf(out, "%.6s%s\0\n", cs->param, cs->msg);
	sprintf(outStr, "%.6s%s\r\n", cs->param, cs->msg);

	//DEBUG OUTPUT:
	//xprintf("CH PARAM:%.6s,MSG:%s\n", cs->param, cs->msg);
}

