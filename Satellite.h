/*
 * Satellite.h
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#ifndef SATELLITE_H_
#define SATELLITE_H_

#include "rodos.h"
#include "topics.h"
#include "PID/AnglePID.h"
#include "PID/RotPID.h"

class Satellite : public Thread {
private:
	uint64_t periode;
	SkyNetMode mode;

	AnglePID anglePID;
	RotPID rotPID;
public:
	Satellite(const char* name, uint64_t periode);
	void init(void);
	void run(void);
	void setPeriode(uint64_t periode);
	void setMode(SkyNetMode mode);
	SkyNetMode getCurrentMode(void);
	void switchMode(void);
	void handleModePeriodic(void);
};

#endif /* SATELLITE_H_ */
