/*
 * topics.h
 *
 *  Created on: 20.11.2014
 *      Author: holger
 */

#ifndef SKYNET_TOPICS_H_
#define SKYNET_TOPICS_H_

#include <cmath>
#include <stdio.h>

#define STD_PERIOD 100 * MILLISECONDS
#define PARAM_LENGTH 6
#define BIAS_BUFFER_SIZE 100
#define BUFFER_SIZE 64

extern bool DEBUG; //one-time status messages
extern bool DBGOUT; //periodic, fast outputs

//structs
//struct CommStruct{
//	char param[6];
//	char * msg;
//};
struct CommStruct{
	char param[6];
	char msg[64];
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

enum SkyNetMode{
	STDNBY,
	ROTMOD,
	COMPAS,
	SUNFIN,
	MISION,
	SkyNetModeCount, // <- keep this as last item
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
	ORIENT,
	SkyNetTMTypeCount,  // <- keep this as last item
};

//topic IDs
enum SkyNetTopicID{
	TelecommandoTopicID = 23456,
	UserButtonTopicID = 34567,
};

//TM
extern Topic<CommStruct> tmTopic;

extern Topic<uint32_t> lightTopic;
extern Topic<RawVector3D> gyroTopic;
extern Topic<RawVector3D> accTopic;
extern Topic<RawVector3D> magTopic;
extern Topic<float> tempTopic;
extern Topic<float> orientationTopic;
//TC
extern Topic<CommStruct> tcTopic;

#endif /* SKYNET_TOPICS_H_ */
