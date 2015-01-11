/*
 * TC.h
 *
 *  Created on: 17.12.2014
 *      Author: holger
 */

#ifndef TC_H_
#define TC_H_

#include "rodos.h"
#include "topics.h"
#include "Hardware/IMU.h"
#include "Hardware/LightSensor.h"
#include "Hardware/MotorThread.h"
#include "TM.h"
#include "Satellite.h"

class TC : public Thread, public Subscriber{
private:
	IMU *imu;
	LightSensor *ls;
	MotorThread *mt;

	void handlePacket(CommStruct *cs);
	void changeMode(SkyNetMode mode);
	bool paramIsEqualTo(CommStruct *cs, const char* param);
	bool msgIsEqualTo(CommStruct *cs, const char* msg);
public:
	TC(const char* name, IMU *imu, LightSensor *ls, MotorThread *mt);
	void init();
	void run();
	long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo);
};

#endif /* TC_H_ */
