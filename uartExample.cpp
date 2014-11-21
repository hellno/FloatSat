/*!
 * \file    uartExample.cpp
 *
 * \date    30.10.2012
 * \author  Michael Ruffer
 *
 * \brief   Shows how to use RODOS HAL_UART. To run this example connect your PC
 *          to uart2 according hal_uart.cpp of your target
 *          e.g. STM32F4 pin PD5 & PD6
 */
#include "uartExample.h"

HAL_UART uart_bluetooth(UART_IDX2);

UartTest u1("UartTest", &uart_bluetooth, 100 * MILLISECONDS);
/***********************************************************************/
