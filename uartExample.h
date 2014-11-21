/*
 * uartExample.h
 *
 *  Created on: 21.11.2014
 *      Author: holger
 */

#ifndef UARTEXAMPLE_H_
#define UARTEXAMPLE_H_

#include "rodos.h"
#include "hal/hal_uart.h"


class UartTest: public Thread {
	uint64_t periode;
	HAL_UART *uart;

public:
	UartTest(const char* name, HAL_UART *uart, uint64_t periode) : Thread(name) {
		this->periode = periode;
		this->uart = uart;
	}

	void init(){
	    uart->init(115200);
	}

	void run() {
		char ch[32];
		const char string[] = "character received: ";
		TIME_LOOP(0,periode){
		    int32_t retVal = uart->read(ch,32);
			if (retVal > 0){
				write(string,sizeof(string)-1);
				write(ch,retVal);
				write("\n\r",2);
			}
		}
	}

	void write(const char *buf, int size){
	    int sentBytes=0;
	    int retVal;
	    while(sentBytes < size){
	        retVal = uart->write(&buf[sentBytes],size-sentBytes);
	        if (retVal < 0){
	            PRINTF("UART sent error\n");
	        }else{
	            sentBytes+=retVal;
	        }
	    }
	}
};

#endif /* UARTEXAMPLE_H_ */
