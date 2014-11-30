/*
 * CallHome.h
 *
 *  Created on: 30.11.2014
 *      Author: holger
 */

#ifndef CALLHOME_H_
#define CALLHOME_H_

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "hal/hal_uart.h"

#include "topics.h"

class CallHome :  public Thread{
private:
	HAL_UART *uart;
	uint64_t period;
	bool active;
public:
	CallHome(const char* name, HAL_UART *uart, uint64_t periode);
	CallHome();
	void init();
	void run();
	void setPeriod(uint64_t period);
	void startCalling();
	void stopCalling();
	bool isCalling();
};

#endif /* CALLHOME_H_ */
