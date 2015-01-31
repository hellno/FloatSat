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
#include "Hardware/LightSensor.h"
#include "Hardware/IMU.h"
#include "Hardware/MotorThread.h"
#include "Hardware/Battery.h"
#include "TC.h"
#include "Satellite.h"
#include "TM.h"
#include "Camera/Camera.h"

static Application module01("FloatSat", 2001);

HAL_UART uart_usb(UART_IDX3);
HAL_UART uart_bt(UART_IDX2);
HAL_I2C light_i2c(I2C_IDX1);
HAL_I2C imuI2C(I2C_IDX2);
HAL_ADC adc(ADC_IDX1);

CommHandler ch("CommHandler", &uart_usb, 20 * MILLISECONDS);

IMU imu("IMU", 100 * MILLISECONDS);
LightSensor ls("LightSensor", &light_i2c, STD_PERIOD);
MotorThread mt("motorThread");
Battery battery("Battery", STD_PERIOD, &adc);

Camera camera("Camera");

TM tm("tmHandler", 20 * MILLISECONDS);
TC tc("tcHandler", &imu, &ls, &mt, &camera);

Satellite skyNet("SkyNet", 50 * MILLISECONDS);
