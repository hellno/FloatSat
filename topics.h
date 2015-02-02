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

#define PID_ERROR_THRESHOLD 5

#define BATTERY_CHANNEL ADC_CH_000 		//= PA0/PA0/PA0
#define SOLAR_PANEL1_CHANNEL ADC_CH_001 //= PA1/PA1/PA1
#define SOLAR_PANEL2_CHANNEL ADC_CH_002 //= PA2/PA2/PA2

#define SOLAR_PANEL_RESISTOR 1.0f

#define MOTOR_SPEED_LIMIT 300

extern bool DEBUG; //one-time status messages
extern bool DBGOUT; //periodic, fast outputs

//structs
struct CommStruct{
	char param[7];
	char msg[64];
};

struct RawVector2D{
	int16_t x;
	int16_t y;
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

enum PIDConstant{
	P,
	I,
	D,
};

//topic IDs
enum SkyNetTopicID{
	TelecommandoTopicID = 23456,
	UserButtonTopicID = 34567,
};
//Hardware
extern Topic<int16_t> motorSpeedTopic;

//TM
extern Topic<CommStruct> tmTopic;

extern Topic<uint32_t> lightTopic;
extern Topic<Vector3D> gyroTopic;
extern Topic<RawVector3D> accTopic;
extern Topic<RawVector3D> magTopic;
extern Topic<float> tempTopic;
extern Topic<float> orientationTopic;
extern Topic<float> yawAngTopic;
extern Topic<RawVector2D> cameraTargetTopic;
extern Topic<float> batteryPercentageTopic;
extern Topic<float> batteryVoltageTopic;
extern Topic<float> solarpanelChargeTopic;
extern Topic<float> pidErrorTopic;
extern Topic<float> pidOutputTopic;
extern Topic<bool> cameraFireTopic;


//TC
extern Topic<CommStruct> tcTopic;

#endif /* SKYNET_TOPICS_H_ */
