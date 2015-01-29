/*
 * CommHandler.h
 *
 *  Created on: 21.11.2014
 *      Author: holger
 */

#ifndef SKYNET_COMMHANDLER_H_
#define SKYNET_COMMHANDLER_H_

#include "rodos.h"
#include "topics.h"
#include <stdio.h>
#include "hal.h"
#include "hal/hal_uart.h"

class CommHandler : public Thread{
private:
	HAL_UART *uart;
	uint64_t periode;

public:
	CommHandler(const char* name, HAL_UART *uart, uint64_t periode);
	void init();
	void run();
	bool parseStringToPacket(char * str, int size, CommStruct* cs);
	void parsePacketToString(char * outStr, CommStruct *cs, bool debug);
};

#endif /* SKYNET_COMMHANDLER_H_ */
