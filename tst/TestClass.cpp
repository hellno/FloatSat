/*
 * TestClass.cpp
 *
 *  Created on: 17.11.2014
 *      Author: holger
 */
#include "TestClass.h"

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062

HAL_GPIO ledg(LED_GREEN);
HAL_GPIO ledo(LED_ORANGE);
HAL_GPIO ledr(LED_RED);

TestClass::TestClass(const char* name) : Thread(name){

}

void TestClass::init(){
	debugMsg("TestClass INIT MSG\n");
}

void TestClass::run(){
	while(1){
		if(ledg.readPins()==0)
		   ledg.setPins(1);
	   else
		   ledg.setPins(0);

		debugMsg("HELP, I'M TRAPPED IN A MICROCONTROLLER!\n");

		suspendCallerUntil(NOW()+500*MILLISECONDS);
	}
}

void TestClass::debugMsg(const char* msg){
	xprintf("Debug %s\n", msg);
}
