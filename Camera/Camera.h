/*
 * Camera.h
 *
 *  Created on: 12.01.2015
 *      Author: Andy
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "rodos.h"
#include "hal.h"
#include "../topics.h"

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

#include "Peripheral/Dcmi.h"
#include "Peripheral/Sccb.h"

#include "Peripheral/Misc/initRegister.h"


#define BT_UART UART_IDX2
#define USB_UART UART_IDX3

#define WIDTH						160
#define HEIGHT						120

#define CAPTUREMODE					DCMI_CaptureMode_SnapShot
#define FRAMERATE					DCMI_CaptureRate_All_Frame
//#define CAPTUREMODE				DCMI_CaptureMode_Continuous
//#define FRAMERATE					DCMI_CaptureRate_1of4_Frame
#define DCMI_DR_ADDRESS      		0x50050028
#define IMAGESIZE					(HEIGHT*WIDTH*2)
#define THRESHOLD					254


class Camera: public Thread {
private:
	Dcmi dcmi;
	Sccb sccb;
	HAL_GPIO ledg;
	HAL_GPIO reset;
	HAL_GPIO power;

	uint8_t DCMI_Buffer[IMAGESIZE];

	int targetX;
	int targetY;
	RawVector2D target;

	bool active;

	void InitOV7670();
	void delayx(unsigned int ms);
public:
	Camera(const char* name);
	void init();
	void run();
	uint8_t* getPicture();
	void sendPicture();
	void Capture();

	void DetectSatellite();
	void turnOn(void);
	void turnOff(void);
};

#endif /* CAMERA_H_ */
