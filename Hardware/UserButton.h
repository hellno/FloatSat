/*
 * UserButton.h
 *
 *  Created on: 18.12.2014
 *      Author: holger
 */

#ifndef USERBUTTON_H_
#define USERBUTTON_H_

#include "rodos.h"
#include "../topics.h"
#include "hal/hal_gpio.h"

class UserButton: public Thread{
private:
	HAL_GPIO userButton;
public:
	UserButton(const char* name);
	void run();
	void init();
};

#endif /* USERBUTTON_H_ */
