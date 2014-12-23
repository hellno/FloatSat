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
#include "IMU.h"
#include "MotorThread.h"
#include "TC.h"
#include "UserButton.h"

static Application module01("FloatSat", 2001);

#define STD_PERIOD 100 * MILLISECONDS

HAL_UART uart_usb(UART_IDX3);
HAL_I2C light_i2c(I2C_IDX3);
HAL_I2C imuI2C(I2C_IDX2);

CommHandler ch("CommHandler", &uart_usb, STD_PERIOD);

IMU imu("IMU");
TC tcHandler("tcHandler", tc);
//LightSensor ls("LightSensor", &light_i2c, STD_PERIOD);
//MotorThread motorThread("motorThread");
//TC buttonHandler("buttonHandler", buttonTopic);
//UserButton button("userButtonThread");
