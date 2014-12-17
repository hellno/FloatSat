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

class TC : public Thread{
private:
	void handlePacket(CommStruct *cs);
	void changeMode(SkyNetModes mode);
	bool paramIsEqualTo(CommStruct *cs, const char* param);
public:
	TC(const char* name);
	void init();
	void run();
};

#endif /* TC_H_ */
