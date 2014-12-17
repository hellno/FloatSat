/*
 * CommHandler.h
 *
 *  Created on: 21.11.2014
 *      Author: holger
 */

#ifndef COMMHANDLER_H_
#define COMMHANDLER_H_

#include "rodos.h"
#include "topics.h"
#include <stdio.h>
#include "hal.h"
#include "hal/hal_uart.h"
#include <string>

#include <stdlib.h>

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
	void sendPacket(SkyNetTMType paramType, char* msg);
};

#endif /* COMMHANDLER_H_ */
