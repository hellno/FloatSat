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

	float prevError;
	int8_t desRot;
public:
	RotPID(void);
	void setDestinationRotation(int8_t rotation);
	void run(void);
	void setP(float p);
	void setI(float i);
	void setD(float d);

	float getP(void);
	float getI(void);
	float getD(void);
};

#endif /* ROTPID_H_ */
