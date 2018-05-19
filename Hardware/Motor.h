/*
 * Motor.h
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "rodos.h"
#include "../topics.h"
#include "hal/hal_i2c.h"
#include "hal/hal_gpio.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#define GPIO_INA GPIO_036 //=PC04
#define GPIO_INB GPIO_017 //=PB01
#define GPIO_PWM GPIO_060 //=PD12

#define GPIO_HBRIDGE GPIO_066 //=PE02

class Motor {
private:
	void dbgStatus();
	int16_t duty_cycle;
public:
	void init();
	void startMotor();
	void stopMotor();
	void setSpeed(int16_t speed);
	int16_t getSpeed(void);
	void handleDirection(int16_t duty_cycle);
};

#endif /* MOTOR_H_ */
