/*
 * TM.h
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#ifndef TM_H_
#define TM_H_
#include "rodos.h"
#include "topics.h"

class TM : public Thread{
private:
	uint64_t periode;
	bool active;
public:
	TM(const char* name, uint64_t periode);
	void init();
	void run();
	void setPeriode(uint64_t periode);
	void sendHousekeepingData(void);


	void turnOn(void);
	void turnOff(void);
	bool isOn(void);
};

#endif /* TM_H_ */
