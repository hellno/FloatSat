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

enum SkyNetModes{
	STDNBY,
	ROTMOD,
	COMPAS,
	SUNFIN,
	MISION,
};

enum SkyNetTMType{
	GYRDAT,
	YAWANG,
	ACCDAT,
	MAGDAT,
	LGHTSN,
	TEMPLS,
	CURBAT,
	CURMOT,
	VOLBAT,
	VOLMOT,
	PWMSIG,
	ROTVEL,
};

//topics
//TM
extern Topic<CommStruct> tc;
extern Topic<uint32_t> lightTopic;
extern Topic<RawVector3D> gyroTopic;
extern Topic<RawVector3D> accTopic;
extern Topic<RawVector3D> magTopic;
extern Topic<uint8_t> tempTopic;

//TC
extern Topic<uint8_t> motorSpeedTopic;

//to access and process incoming commands
//create a class and add..
//CommBuffer<CommStruct> tcfifo;
//Subscriber sub(tc, tcfifo);

#endif /* TOPICS_H_ */
