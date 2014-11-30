/*
 * CallHome.cpp
 *
 *  Created on: 30.11.2014
 *      Author: holger
 */

#include "CallHome.h"
#include "helper.h"


#define BUFFERSIZE 128

CallHome::CallHome(const char* name, HAL_UART *uart, uint64_t period) : Thread(name){
	this->period = period;
	this->uart = uart;
	this->active = true;
}

CallHome::CallHome() : Thread(){
	xprintf("doing stuff");
}

void CallHome::init(){
	uart->init(115200);
}

void CallHome::run(){
	char buf[BUFFERSIZE];
	char tcBuf[BUFFERSIZE];
	CommStruct tempTCStruct;
	uint32_t lux = 0;

	while(active){
		//get lux -> CREATE SUSCRIBER
		uart->write("Lux: %d\n", lux);

		if(/*new TC available)*/true){
			int len = parsePacketToString(buf, &tempTCStruct, BUFFERSIZE);
			uart->write(buf, len);
		}
		suspendCallerUntil(NOW() + period);
	}
}

void CallHome::startCalling(){
	active=true;
}

void CallHome::stopCalling(){
	active=false;
}

bool CallHome::isCalling(){
	return active;
}

void CallHome::setPeriod(uint64_t period){
	this->period = period;
}
