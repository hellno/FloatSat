/*
 * AnglePID.h
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#ifndef ANGLEPID_H_
#define ANGLEPID_H_

#include "rodos.h"
#include "../topics.h"

class AnglePID {
private:
	float P;
	float I;
	float D;

	float prevError;
	uint8_t desAngle;
public:
	AnglePID(void);
	void setDestinationAngle(uint8_t angle);
	void run(void);
	void setP(float p);
	void setI(float i);
	void setD(float d);

	float getP(void);
	float getI(void);
	float getD(void);
};

#endif /* ANGLEPID_H_ */
