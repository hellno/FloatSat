/*
 * topics.h
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#define PARAM_LENGTH 6
#define BIAS_BUFFER_SIZE 100

//structs
struct CommStruct{
	char param[6];
	char * msg;
};


struct RawVector3D{
	int16_t x;
	int16_t y;
	int16_t z;
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
enum SkyNetTopicID{
	TelecommandoTopicID = 23456,
	UserButtonTopicID = 34567,
};
//TM
extern Topic<CommStruct> tc;
extern Topic<uint32_t> lightTopic;
extern Topic<RawVector3D> gyroTopic;
extern Topic<RawVector3D> accTopic;
extern Topic<RawVector3D> magTopic;
extern Topic<uint8_t> tempTopic;

//TC
extern Topic<uint8_t> motorSpeedTopic;

//Hardware
extern Topic<bool> buttonTopic;

//to access and process incoming commands
//create a class and add..
//CommBuffer<CommStruct> tcfifo;
//Subscriber sub(tc, tcfifo);

#endif /* TOPICS_H_ */
