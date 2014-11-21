/*
 * TestClass.h
 *
 *  Created on: 21.11.2014
 *      Author: holger
 */

#ifndef TESTCLASS_H_
#define TESTCLASS_H_

#include "rodos.h"

class TestClass : public Thread{

private:

public:
	TestClass(const char* name);
	virtual void init();

	void run();

	void debugMsg(const char* msg);
};

#endif /* TESTCLASS_H_ */
