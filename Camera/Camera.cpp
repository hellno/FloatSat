/*
 * Template.cpp
 *
 * Created on: 25.06.2014
 * Author: Atheel Redah
 *
 */

#include "Camera.h"
#include "../TM.h"

Topic<RawVector2D> cameraTargetTopic(-1, "camera target");
Topic<bool> cameraFireTopic(-1, "camera fire");

extern TM tm;

Camera::Camera(const char* name, HAL_UART uart) :
		Thread(name),
		dcmi(IMAGESIZE, (uint32_t) DCMI_Buffer, FRAMERATE, CAPTUREMODE),
<<<<<<< HEAD
		ledg(GPIO_061), //PD13
		reset(GPIO_010), //PA10
		power(GPIO_033), //PC01
		targetX(0), targetY(0) {
=======
		ledo(GPIO_061),
		reset(GPIO_010),
		power(GPIO_033),
		tmUart(uart),
		sendPic(false){
>>>>>>> origin/andy
		active = false;
		processData = false;
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
	ledo.init(true);
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
	sendPic = true;
	//tm.turnOn();
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
				DCMI_Buffer[2 * x + 2 * y * WIDTH] = 255;
			} else {
				//DCMI_Buffer[2 * x + 2 * y * WIDTH] = 0;
			}
		}
	}

	// Optimize here ------
	/*long sum1 = 0;
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
//	xprintf("Horizontal sum 1: %d, sum 2: %d, meanWidth: %d\n", sum1, sum2, meanWidth);

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
	}*/
//	xprintf("Vertical sum 1: %d, sum 2: %d meanHeight: %d\n", sum1, sum2, meanHeight);

    // Optimize here ------
    int counter = 0;
    int sum1 = 0;
    int sum2 = 0;
    for(int x = 0; x < WIDTH; x++) {
        sum1 += horizontalLine[x];
    }
    int first_quarter = 0;
    int third_quarter = 0;
    int targetX = 0;
    int spanX = 0;
    while(sum2<Q3*sum1) {
        sum2 += horizontalLine[counter];
        counter++;
        if((Q1*sum1<sum2)&&(first_quarter==0)){
            first_quarter = counter;
        }
        if((HALF*sum1<sum2)&&(targetX==0)) {
            targetX = counter;
        }
        if((Q3*sum1<sum2)&&(third_quarter==0)) {
            third_quarter = counter;
        }
    }
    spanX = third_quarter - first_quarter;

    counter = 0;
    sum1 = 0;
    sum2 = 0;
    for(int y = 0; y < HEIGHT; y++) {
        sum1 += verticalLine[y];
    }
    first_quarter = 0;
    third_quarter = 0;
    int targetY = 0;
    int spanY = 0;
    while(sum2<Q3*sum1) {
        sum2 += verticalLine[counter];
        counter++;
        if((Q1*sum1<sum2)&&(first_quarter==0)){
            first_quarter = counter;
        }
        if((HALF*sum1<sum2)&&(targetY==0)) {
            targetY = counter;
        }
        if((Q3*sum1<sum2)&&(third_quarter==0)) {
            third_quarter = counter;
        }
    }
    spanY = third_quarter - first_quarter;
    // -----------------------

    bool fireAngle = spanX>2*spanY;
	target.y = targetX;
	target.x = targetY;
	cameraTargetTopic.publish(target);
	cameraFireTopic.publish(fireAngle);
	xprintf("Target: x:%d, y:%d\n", target.x, target.y);
	xprintf("Fire: %d", fireAngle);
	// -----------------------*/
}

void Camera::ProcessData() {
	processData = true;
}

void Camera::run() {

	while (1) {
		if (processData) {
		if (sendPic) {
			tm.turnOff();
			char tmpVal[4];
			tmUart.write("CAMERA", 6);
			for (int i = 0; i < IMAGESIZE; i += 2) {
				sprintf(tmpVal, "%03u", DCMI_Buffer[i]);
				tmUart.write(tmpVal, 4);
				while (!tmUart.isWriteFinished()) {
				}
			}
			tmUart.write("CAMEND", 6);
			sendPic = false;
		}


			processData = false;
			DetectSatellite();

			if (active) { // Continue Captureing/Processing if still active
				Capture();
			}

			suspendCallerUntil(NOW()+200*MILLISECONDS);
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
	//ledo.setPins(1);
	xprintf("Cam active\n");
	Capture();
}
void Camera::turnOff(void){
	this->active = false;
	xprintf("Cam inactive\n");
	//ledo.setPins(0);
}
/***********************************************************************/
