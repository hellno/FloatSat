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

static Application module01("FloatSat", 2001);

#define IMU_I2C I2C_IDX2

HAL_UART uart_usb(UART_IDX3);

CommHandler ch("CommHandler", &uart_usb, 100 * MILLISECONDS);
