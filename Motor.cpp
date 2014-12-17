/*
 * Motor.cpp
 *
 *  Created on: 16.12.2014
 *      Author: holger
 */

#include "Motor.h"

HAL_GPIO inAGPIO(GPIO_INA);
HAL_GPIO inBGPIO(GPIO_INB);
HAL_GPIO pwmGPIO(GPIO_PWM);

//activate h-bridge
HAL_GPIO enableHBRIDGE(GPIO_HBRIDGE);

HAL_PWM pwm(PWM_IDX12); // PWM_IDX12: PB06 -> PD12

void Motor::init(){
	inAGPIO.init(true,1,1);
	inBGPIO.init(true,1,0);

	pwmGPIO.init(false,1,0);
	enableHBRIDGE.init(true,1,0);

	pwm.init();
	pwm.write(50);
}

void Motor::startMotor(){
	enableHBRIDGE.setPins(1);
	//xprintf("start motor\n");
	//dbgStatus();
}

void Motor::stopMotor(){
	enableHBRIDGE.setPins(0);
	xprintf("stop motor\n");
}

void Motor::setSpeed(uint8_t percentage){
	if(percentage < 0 || percentage > 100)
		return;

	pwm.write(percentage);
}

void Motor::dbgStatus(){
	uint32_t statusA, statusB, statusC;
	statusA = inAGPIO.readPins();
	statusB = inBGPIO.readPins();
	statusC = enableHBRIDGE.readPins();

	xprintf("status - A: %d, B: %d, DCDC: %d\n", statusA, statusB, statusC);
}
