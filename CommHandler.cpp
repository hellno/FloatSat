/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

CommHandler::
CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) : Thread(name){
			this->periode = periode;
			this->uart = uart;
		}

		void CommHandler::init(){
			uart->init(115200);
		}

		void CommHandler::run(){
			char buf[BUFFER_SIZE];
			TIME_LOOP(0, periode){

				if(uart->isDataReady()){
					xprintf("DATA IS READY, BABY");
					uart->read(buf, 512);
					CommStruct cs = parseStringToPacket(buf);

					tc.publish(cs);

					xprintf(parsePacketToString(cs));
				}
			}
		}

	CommStruct CommHandler::parseStringToPacket(char * str){
		CommStruct cs; // = new CommStruct();

		int size = strlen(str);
		const char * newline_char = "\r\n";
		const char * last_char = &str[size - 1];

		if(size < 6)
			return cs;

		if(strcmp(last_char, newline_char) != 0){
			xprintf("parseStrToPkt: last char != newline, was %c", last_char);
			return cs;
		}

		printf(cs.param, "%.5s", str);
		printf(cs.msg, substring(str, 6, size));

		return cs;

	}

	char * CommHandler::parsePacketToString(CommStruct cs){
		char * out;
		sprintf(out, "PARAM: %s, MSG: %s", cs.param, cs.msg);
		return out;
	}

	char* CommHandler::substring(char* arr, int begin, int len){
	    //WATCH OUT, MAYBE UNSAFE TO CREATE RES WITHOUT ANY LIMIT
		char* res;
		//char* tst = (char *) malloc (len);
	    for (int i = 0; i < len; i++){
	        res[i] = *(arr + begin + i);
	    }
	    res[len] = 0;
	    return res;
	}


