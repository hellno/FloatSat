/*
 * TM.cpp
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#include "TM.h"

#define FIFO_SIZE 2

Fifo<uint32_t, FIFO_SIZE> lightFifo;
Fifo<Vector3D, FIFO_SIZE> gyroFifo;
Fifo<RawVector3D, FIFO_SIZE> accFifo;
Fifo<RawVector3D, FIFO_SIZE> magFifo;
Fifo<float, FIFO_SIZE> tempFifo;
Fifo<float, FIFO_SIZE> orientationFifo;
Fifo<float, FIFO_SIZE> yawAngleFifo;
Fifo<RawVector2D, FIFO_SIZE> cameraTargetFifo;
Fifo<float, FIFO_SIZE> batVolFifo;
Fifo<int16_t, FIFO_SIZE> motorSpeedFifo;
Fifo<float, FIFO_SIZE> pidErrorFifo;
Fifo<float, FIFO_SIZE> pidOutputFifo;

Subscriber motorSpeedSubscriber(motorSpeedTopic, motorSpeedFifo, "motorSpeedSub");
Subscriber lightSubscriber(lightTopic, lightFifo, "lightSub");
Subscriber gyroSubscriber(gyroTopic, gyroFifo, "gyroSub");
Subscriber accSubscriber(accTopic, accFifo, "accSub");
Subscriber magSubscriber(magTopic, magFifo, "magSub");
Subscriber tempSubscriber(tempTopic, tempFifo, "tempSub");
Subscriber orientationSubscriber(orientationTopic, orientationFifo, "orientationSub");
Subscriber yawAngleSubscriber(yawAngTopic, yawAngleFifo, "yawAngleSub");
Subscriber cameraTargetSubscriber(cameraTargetTopic, cameraTargetFifo, "cameraTargetSub");
Subscriber batteryVoltageSubscriber(batteryVoltageTopic, batVolFifo, "batteryVoltageSub");
Subscriber pidErrorSubscriber(pidErrorTopic, pidErrorFifo, "pidErrorSub");
Subscriber pidOutputSubscriber(pidOutputTopic, pidOutputFifo, "pidOutputSub");

char buf[BUFFER_SIZE];

TM::TM(const char* name, uint64_t periode) : Thread(name){
	this->periode = periode;
	this->active = true;
}

void TM::init(void){

}

void TM::run(void){
	//CommStruct cs;

	while(1){
		suspendCallerUntil(NOW() + periode);
		if(active){
			sendHousekeepingData();
		}
	}
}

void vector_to_string(char* str, Vector3D *a){
	sprintf(str, "%.2f,%.2f,%.2f", a->x, a->y, a->z);
}

void raw_vector_to_string(char* str, RawVector3D *a){
	sprintf(str, "%d,%d,%d", a->x, a->y, a->z);
}

void raw_vector_to_string(char* str, RawVector2D *a){
	sprintf(str, "%d,%d", a->x, a->y);
}

void copyStringToMsg(char* str, CommStruct *cs){
	size_t len = strlen(str);
	if(len < 1)
		return;

	char tmpVal[len];
	tmpVal[len - 1] = '\0';

	for(uint8_t i = 0; i < len; i++){
		*(tmpVal + i) = *(str+i);
	}

	//OLD strncpy(cs->msg, tmpVal, strlen(cs->msg));
	strncpy(cs->msg, tmpVal, len);
}

void raw_vector_to_msg(CommStruct *cs, RawVector3D *a){
	sprintf(buf, "%d,%d,%d\0", a->x, a->y, a->z);

	size_t len = strlen(buf);

	if(len < 1)
		return;


	/* alternative way of copying string
	char tmpVal[len];
	tmpVal[len - 1] = '\0';

	for(uint8_t i = 0; i < len; i++){
		*(tmpVal + i) = *(buf+i);
	}
	*/

	strncpy(cs->msg, buf, len);
}


void TM::sendHousekeepingData(void){
	CommStruct cs;
	RawVector3D tempRawVector;
	Vector3D tempVector;
	RawVector2D tempVector2D;
	uint32_t tempInt;
	int16_t temp16Int;
	float tempFloat;

	/*
	//LGHTSN
	if(lightFifo.get(tempInt)){
		sprintf(cs.param, "%.6s", "LGHTSN");
		sprintf(cs.msg, "%d", tempInt);
		tmTopic.publish(cs);
	}

	//MAGDAT
	if(magFifo.get(tempRawVector)){
		sprintf(cs.param, "%.6s", "MAGDAT");
		raw_vector_to_msg(&cs, &tempRawVector);
		tmTopic.publish(cs);
	}
	//ACCDAT
	if(accFifo.get(tempRawVector)){
		sprintf(cs.param, "%.6s", "ACCDAT");
		raw_vector_to_string(cs.msg, &tempRawVector);
		tmTopic.publish(cs);
	}

	//GYRDAT
	if(gyroFifo.get(tempVector)){
		sprintf(cs.param, "%.6s", "GYRDAT");
		vector_to_string(cs.msg, &tempVector);
		tmTopic.publish(cs);
	}

	//ORIENT
	if(orientationFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "ORIENT");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}

	//TARGET
	if(cameraTargetFifo.get(tempVector2D)){
		sprintf(cs.param, "%.6s", "TARGET");
		raw_vector_to_string(cs.msg, &tempVector2D);
		tmTopic.publish(cs);
	}

	//BATVOL
	if(batVolFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "BATVOL");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}

	//BATPCT
	if(batPercFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "BATPCT");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}

	//SOLPAC
	if(solarpanelCurrentFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "SOLPAI");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}
	*/
	//MOTSPD
	if(motorSpeedFifo.get(temp16Int)){
		sprintf(cs.param, "%.6s", "MOTSPD");
		sprintf(cs.msg, "%d", temp16Int);
		tmTopic.publish(cs);
	}

	//YAWANG
	if(yawAngleFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "YAWANG");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}

	//PIDERR
	if(pidErrorFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "PIDERR");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}
	//PIDOUT
	if(pidOutputFifo.get(tempFloat)){
		sprintf(cs.param, "%.6s", "PIDOUT");
		sprintf(cs.msg, "%.2f", tempFloat);
		tmTopic.publish(cs);
	}
}

void TM::setPeriode(uint64_t periode){
	this->periode = periode;
	if (DEBUG) xprintf("changed satellite period to: %ld\n", this->periode);
}
void TM::turnOn(void){
	active = true;
}
void TM::turnOff(void){
	active = false;
}
bool TM::isOn(void){
	return active;
}

