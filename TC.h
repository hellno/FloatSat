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

class TC : public Thread, public Subscriber{
private:
	const char* name;
	long topicId;

	void handlePacket(CommStruct *cs);
	void changeMode(SkyNetModes mode);
	bool paramIsEqualTo(CommStruct *cs, const char* param);
public:
	TC(const char* name, RODOS::TopicInterface &topic);
	void init();
	void run();
	long put(const long topicId, const long len, const void* data, const NetMsgInfo& netMsgInfo);
};

#endif /* TC_H_ */
