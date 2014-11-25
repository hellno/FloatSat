/*
 * CommHandler.cpp
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */
#include "CommHandler.h"

Topic<CommStruct> tc(-1, "TC");

CommHandler::CommHandler(const char* name, HAL_UART *uart, uint64_t periode) : Thread(name){
			this->periode = periode;
			this->uart = uart;
		}

		void CommHandler::init(){
			uart->init(115200);
		}

		void CommHandler::run(){
			char buf[BUFFER_SIZE + 1];
			buf[BUFFER_SIZE] = '\0';

			TIME_LOOP(0, periode){

				if(uart->isDataReady()){
					xprintf("DATA IS READY, BABY\n");
					uart->read(buf, BUFFER_SIZE);

					CommStruct cs;

					parseStringToPacket(buf, &cs);

					//unsigned int retVal = tc.publish(cs);
					//xprintf("%d retVal from publishing to topic", retVal);
					//xprintf(parsePacketToString(cs));
					xprintf("end of data read\n");
				}
			}
	}

	void CommHandler::parseStringToPacket(char * str, CommStruct* cs){
		xprintf("string in fct(): %s\n", str);

		int size = strlen(str);

		xprintf("char length: %d", size);

		if(size < 6)
			return;

		sprintf(cs->param, "%.5s", str);
		sprintf(cs->msg, substring(str, 6, size));

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



