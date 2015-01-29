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

	float P_factor;
	float I_factor;
	float D_factor;

	float prevError;
	float output;
	float integral;
	float derivative;
	float desAngle;

	float tempVal;
public:
	AnglePID(void);
	void setDestinationAngle(float angle);
	void run(void);
	uint16_t currentOutput(void);

	void setP(float p);
	void setI(float i);
	void setD(float d);

	float getP(void);
	float getI(void);
	float getD(void);
};

#endif /* ANGLEPID_H_ */
