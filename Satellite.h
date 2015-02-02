/*
 * Satellite.h
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#ifndef SKYNET_SATELLITE_H_
#define SKYNET_SATELLITE_H_

#include "rodos.h"
#include "topics.h"
#include "PID/AnglePID.h"
#include "PID/RotPID.h"

class Satellite : public Thread {
private:
	HAL_PWM firePWM; // PWM_IDX00 -> PE9
	HAL_GPIO pwmGPIO; // PE9

	uint64_t periode;
	SkyNetMode mode;

	HAL_GPIO burnwire; // PE14

	AnglePID anglePID;
	RotPID rotPID;
	int16_t tempValue;
public:
	Satellite(const char* name, uint64_t periode);
	void init(void);
	void run(void);
	void setPeriode(uint64_t periode);
	void setMode(SkyNetMode mode);
	SkyNetMode getCurrentMode(void);
	void switchMode(void);
	void handleModePeriodic(void);
	void setDestinationAngle(uint8_t angle);
	void setDestinationRotation(int8_t rot);
	void setAnglePIDConst(PIDConstant select, float val);
	void setRotPIDConst(PIDConstant select, float val);
	void sendPicture(void);
	void camDetect(void);
	void capturePicture(void);
	void fireNet(void);
	void deploySolarArray(void);
};

#endif /* SKYNET_SATELLITE_H_ */
