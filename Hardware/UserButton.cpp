/*
 * UserButton.cpp
 *
 *  Created on: 18.12.2014
 *      Author: holger
 */

#include "UserButton.h"

#define BUTTON_PIN	GPIO_000 //PA00

Topic<bool> buttonTopic(UserButtonTopicID, "userButton");
bool previousButtonState;
UserButton::UserButton(const char* name) : Thread(name){

}
void UserButton::init(){
	userButton = HAL_GPIO(BUTTON_PIN);
	userButton.init();
	previousButtonState = userButton.readPins();
}

void UserButton::run(){
	bool currentState;

	TIME_LOOP(0, 50 * MILLISECONDS){
		currentState = userButton.readPins();

		if(currentState != previousButtonState){
			buttonTopic.publish(currentState);
			previousButtonState = currentState;
		}
	}
}
