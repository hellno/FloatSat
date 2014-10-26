/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "math.h"

static Application module01("Template", 2001);

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define IMU_I2C I2C_IDX2

HAL_GPIO ledg(LED_GREEN);

struct TelecommandData {

};

CommBuffer<TelecommandData> TelecommandFifo;
Topic<TelecommandData> TelecommandTopic(-1, "Telecommands");
Subscriber Sub1(TelecommandTopic,TelecommandFifo);


class Telecommand: public Thread {

public:

	Telecommand(const char* name) : Thread(name) {
	}
	void init() {
		ledg.init(true);
	}

	void run(){

               while (1) {

            	   if(ledg.readPins()==0) ledg.setPins(1);
            	   else ledg.setPins(0);
            	   suspendCallerUntil(NOW()+100*MILLISECONDS);
               }

	}

};
Telecommand Telecommand("Telecommand");

/***********************************************************************/
