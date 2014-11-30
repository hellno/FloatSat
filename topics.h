/*
 * topics.h
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#define PARAM_LENGTH 6

//structs
struct CommStruct{
	char param[6];
	char * msg;
};

struct SatState{
	unsigned int angle;
};

//topics
extern Topic<CommStruct> tc;
extern Topic<SatState> st;
extern Topic<uint32_t> lightTopic;

//to access and process incoming commands
//create a class and add..
//CommBuffer<CommStruct> tcfifo;
//Subscriber sub(tc, tcfifo);

#endif /* TOPICS_H_ */
