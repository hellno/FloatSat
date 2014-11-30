/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "rodos.h"

#include <stdio.h>
#include "CommHandler.h"
#include "LightSensor.h"
#include "CallHome.h"

static Application module01("FloatSat", 2001);

#define LIGHT_I2C I2C_IDX3
#define IMU_I2C I2C_IDX2

#define STD_PERIOD 100 * MILLISECONDS

HAL_UART uart_usb(UART_IDX3);
HAL_I2C light_i2c(LIGHT_I2C);

CommHandler ch("CommHandler", &uart_usb, STD_PERIOD);
LightSensor ls("LightSensor", &light_i2c, STD_PERIOD);
CallHome hC = CallHome("CallMeMaybe", &uart_usb,  STD_PERIOD);
