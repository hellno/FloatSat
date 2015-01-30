/*
 * RotPID.h
 *
 *  Created on: 12.01.2015
 *      Author: holger
 */

#ifndef ROTPID_H_
#define ROTPID_H_

#include "rodos.h"
#include "../topics.h"

class RotPID {
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
	int16_t desRot;

	float period;

	Vector3D tempVal;
public:
	RotPID(void);
	void setDestinationRotation(int8_t rot);
	void run(void);
	uint16_t currentOutput(void);
	void setPeriod(float seconds);

	void setP(float p);
	void setI(float i);
	void setD(float d);

	float getP(void);
	float getI(void);
	float getD(void);
};

#endif /* ROTPID_H_ */
