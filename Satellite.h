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


class Satellite : public Thread {
private:
	uint64_t periode;
	SkyNetMode mode;
public:
	Satellite(const char* name, uint64_t periode);
	void init(void);
	void run(void);
	void setPeriode(uint64_t periode);
	void setMode(SkyNetMode mode);
	SkyNetMode getCurrentMode(void);
	void switchMode(void);
};

#endif /* SATELLITE_H_ */
