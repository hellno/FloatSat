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

struct RawVector3D{
	uint16_t x;
	uint16_t y;
	uint16_t z;
};

struct Vector3D{
	float x;
	float y;
	float z;
};

struct IMUData{
	Vector3D gyro;
};

//topics
extern Topic<CommStruct> tc;
extern Topic<SatState> st;
extern Topic<uint32_t> lightTopic;
extern Topic<Vector3D> gyroTopic;

//to access and process incoming commands
//create a class and add..
//CommBuffer<CommStruct> tcfifo;
//Subscriber sub(tc, tcfifo);

#endif /* TOPICS_H_ */
