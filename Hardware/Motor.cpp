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

bool isRunning = false;

void Motor::init(){
	inAGPIO.init(true,1,1);
	inBGPIO.init(true,1,0);

	pwmGPIO.init(false,1,0);
	enableHBRIDGE.init(true,1,0);

	pwm.init(1000,1000);
	pwm.write(50);
}

void Motor::startMotor(){
	enableHBRIDGE.setPins(1);
	isRunning = true;
	//xprintf("start motor\n");
	//dbgStatus();
}

void Motor::stopMotor(){
	enableHBRIDGE.setPins(0);
	isRunning = false;
	xprintf("stop motor\n");
}

void Motor::setSpeed(uint16_t duty_cycle){
	if(duty_cycle <= 0)
		this->stopMotor();

	if(!isRunning)
		this->startMotor();

	if(duty_cycle > 1000)
		duty_cycle = 1000;


	pwm.write(duty_cycle);
	if(DEBUG) xprintf("set new motor speed to %d%\n", duty_cycle);
}

void Motor::dbgStatus(){
	uint32_t statusA, statusB, statusC;
	statusA = inAGPIO.readPins();
	statusB = inBGPIO.readPins();
	statusC = enableHBRIDGE.readPins();

	xprintf("status - A: %d, B: %d, DCDC: %d\n", statusA, statusB, statusC);
}
