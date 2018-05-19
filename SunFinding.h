/*
 * SunFinding.h
 *
 *  Created on: 01.02.2015
 *      Author: holger
 */

#ifndef SUNFINDING_H_
#define SUNFINDING_H_

#include "rodos.h"
#include "topics.h"

#define LOWER_LIMIT_FACTOR 0.8f

class SunFinding : public Thread {
private:
	uint32_t maxLux;
	float angleAtMaxLux;
	bool active;
	bool sunFound;
public:
	SunFinding(const char* name);
	void init(void);
	void run(void);
	uint32_t getAvgLux(void);

	uint32_t getMaxLux(void);
	float getSunAngle(void);

	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
};

#endif /* SUNFINDING_H_ */
