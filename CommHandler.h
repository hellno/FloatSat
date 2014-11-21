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

struct CommStruct{
	char param[6];
	char * msg;
};

#define BUFFER_SIZE 512


class CommHandler : public Thread{
private:
	HAL_UART *uart;
	uint64_t periode;

public:
	CommHandler(const char* name, HAL_UART *uart, uint64_t periode);
	void init();
	void run();
	CommStruct parseStringToPacket(char * str);
	char * parsePacketToString(CommStruct cs);
	char* substring(char* arr, int begin, int len);

};

#endif /* COMMHANDLER_H_ */
