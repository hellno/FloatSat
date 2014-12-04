/*
 * CommHandler.h
 *
 *  Created on: 21.11.2014
 *      Author: holger
 */

#ifndef COMMHANDLER_H_
#define COMMHANDLER_H_

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "hal/hal_uart.h"
#include <string>

#include "topics.h"
#include <stdlib.h>
#include <malloc.h>

class CommHandler : public Thread{
private:
	HAL_UART *uart;
	uint64_t periode;

public:
	CommHandler(const char* name, HAL_UART *uart, uint64_t periode);
	void init();
	void run();
	void parseStringToPacket(char * str, int size, CommStruct* cs);
	void parsePacketToString(char * out, CommStruct *cs);
};

#endif /* COMMHANDLER_H_ */