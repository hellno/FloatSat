/*
 * topics.h
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#include "rodos.h"

//all topics are declared in this file

Topic<CommStruct> tc(-1, "TC");

//to access and process incoming commands
//create a class and add..
//CommBuffer<CommStruct> tcfifo;
//Subscriber sub(tc, tcfifo);

#endif /* TOPICS_H_ */
