/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "Camera.h"

Camera::Camera(const char* name) :
		Thread(name),
		dcmi(IMAGESIZE, (uint32_t) DCMI_Buffer, FRAMERATE, CAPTUREMODE),
		ledg(GPIO_061),
		reset(GPIO_010),
		power(GPIO_033),
		targetX(0), targetY(0) {
		active = false;
}

void Camera::InitOV7670() {
	xprintf("starting InitOV7670 init\n");
	uint16_t x = 0;
	int res = 0;
	res = sccb.ov7670_set(0x12, 0x80);
	res = sccb.ov7670_set(0x12, 0x00);

	while (init_registers[x][0] != 0xFF && init_registers[x][1] != 0xFF) {
		xprintf("init register: status x=%d\n", x);

		res = sccb.ov7670_set((unsigned char) init_registers[x][0],
				(unsigned char) init_registers[x][1]);
		uint8_t read = sccb.ov7670_get((unsigned char) init_registers[x][0]);
		xprintf("SCCB Init %d: reg 0x%x = 0x%x = 0x%x \n", x,
				init_registers[x][0], init_registers[x][1], read);
		if (res) {
			xprintf("ERROR I2C %d\n", res);
		}
		x++;

	}
	xprintf("done with InitOV7670 init\n");
}

void Camera::init() {
	xprintf("starting cam init\n");
	ledg.init(true);
	reset.init(true);
	power.init(true);
	reset.setPins(1);
	power.setPins(0);
	xprintf("Init GPIOs...");
	dcmi.InitGPIO();
	xprintf("Done!\n");
	xprintf("Init DCMI...");
	dcmi.InitDCMI();
	xprintf("Done!\n");
	xprintf("Init I2C...");
	delayx(1000);
	sccb.I2CInit();
	xprintf("Done!\n");
	xprintf("Init OV7670...");
	delayx(1000);
	InitOV7670();
	xprintf("Done!\n");
	xprintf("Enable DCMI...");
	delayx(1000);
	dcmi.EnableDCMI();

	xprintf("Done with cam init!\n");
}

void Camera::sendPicture() {
	xprintf("F");
	xprintf("%d;", targetX);
	xprintf("%d;", targetY);
	for (int i = 0; i < IMAGESIZE; i += 2) {
		xprintf("%u;", DCMI_Buffer[i]);
	}
}

uint8_t* Camera::getPicture() {
	return DCMI_Buffer;
}

void Camera::Capture() {
	DCMI_CaptureCmd(ENABLE);
}


void Camera::DetectSatellite() {
	int horizontalLine[WIDTH];
	int verticalLine[HEIGHT];

	// Init Arrays
	for (int x = 0; x < WIDTH; x++) {
		horizontalLine[x] = 0;
	}
	for (int y = 0; y < HEIGHT; y++) {
		verticalLine[y] = 0;
	}

	int point;
	// Sum up Picture to Lines
	// for(int i = 0; i < IMAGESIZE; i+=2) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if ((int) DCMI_Buffer[2 * x + 2 * y * WIDTH] > THRESHOLD) {
				horizontalLine[x] += (int) DCMI_Buffer[2 * x + 2 * WIDTH * y];
				verticalLine[y] += (int) DCMI_Buffer[2 * x + 2 * WIDTH * y];
				//xprintf("X: %d, Y: %d\n",(int)DCMI_Buffer[2*x + WIDTH*y], (int)DCMI_Buffer[2*x + WIDTH*y]);
				//xprintf("X Sum-up: %d, Y Sum-up: %d\n", horizontalLine[x], verticalLine[y]);
				//xprintf("x: %d, y: %d\n", x,y);
				//DCMI_Buffer[2 * x + 2 * y * WIDTH] = 255;
			} else {
				//DCMI_Buffer[2 * x + 2 * y * WIDTH] = 0;
			}
		}
	}

	// Optimize here ------
	long sum1 = 0;
	long sum2 = 0;
	//  xprintf("Horizontal Line:\n");
	for (int x = 0; x < WIDTH; x++) {
		sum1 += horizontalLine[x];
		//       xprintf("%d\n",horizontalLine[x]);
	}
	int meanWidth = 0;
	while (sum2 < (int) ((float) sum1 / 2.0)) {
		sum2 += horizontalLine[meanWidth];
		meanWidth++;
	}

	sum1 = 0;
	sum2 = 0;
	// xprintf("Vertical Line:\n");
	for (int y = 0; y < HEIGHT; y++) {
		sum1 += verticalLine[y];
		//     xprintf("%d\n",verticalLine[y]);
	}
	int meanHeight = 0;
	while (sum2 < (int) ((float) sum1 / 2.0)) {
		sum2 += verticalLine[meanHeight];
		meanHeight++;
	}

	targetX = meanWidth;
	targetY = meanHeight;
	// -----------------------*/
}

void Camera::run() {

	while (1) {
		if(active){
			suspendCallerUntil(NOW()+2000*MILLISECONDS);
			if (ledg.readPins() == 0) {
				ledg.setPins(1);
				xprintf("capture\n");
				Capture();
				xprintf("sending\n");
			} else {

				// Test Case: Send Pic when target detected, reset target afterwards!
				//DetectSatellite();
				sendPicture();

	//			if((targetX>0)&&(targetY>0)){
	//				sendPicture();
	//				targetX = 0;
	//				targetY = 0;
	//			}


				ledg.setPins(0);
			}
		}
	}

}

void Camera::delayx(unsigned int ms) {
	//4694 = 1 ms
	while (ms > 1) {
		ms--;
		asm("nop");
	}
}


void Camera::turnOn(void){
	this->active = true;
}
void Camera::turnOff(void){
	this->active = false;
}
/***********************************************************************/
