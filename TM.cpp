/*
 * TM.cpp
 *
 *  Created on: 11.01.2015
 *      Author: holger
 */

#include "TM.h"

#define FIFO_SIZE 5

Fifo<uint32_t, FIFO_SIZE> lightFifo;
Fifo<RawVector3D, FIFO_SIZE> gyroFifo;
Fifo<RawVector3D, FIFO_SIZE> accFifo;
Fifo<RawVector3D, FIFO_SIZE> magFifo;
Fifo<float, FIFO_SIZE> tempFifo;
Fifo<float, FIFO_SIZE> orientationFifo;

Subscriber lightSubscriber(lightTopic, lightFifo, "lightSub");
Subscriber gyroSubscriber(gyroTopic, gyroFifo, "gyroSub");
Subscriber accSubscriber(accTopic, accFifo, "accSub");
Subscriber magSubscriber(magTopic, magFifo, "magSub");
Subscriber tempSubscriber(tempTopic, tempFifo, "tempSub");
Subscriber orientationSubscriber(orientationTopic, orientationFifo, "orientationSub");

TM::TM(const char* name, uint64_t periode) : Thread(name){
	this->periode = periode;
	this->active = true;
}

void TM::init(void){

}

void TM::run(void){
	CommStruct cs;

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

void vector_to_string(char* str, RawVector3D *a){
	sprintf(str, "%d,%d,%d", a->x, a->y, a->z);
}


void TM::sendHousekeepingData(void){
	CommStruct cs;
	RawVector3D tempVector;
	uint32_t tempInt;
	float tempFloat;

	//MAGDAT
	magFifo.get(tempVector);
	sprintf(cs.param, "%.6s", "MAGDAT");
	vector_to_string(cs.msg, &tempVector);
	tmTopic.publish(cs);

	//GYRDAT
	gyroFifo.get(tempVector);
	sprintf(cs.param, "%.6s", "GYRDAT");
	vector_to_string(cs.msg, &tempVector);
	tmTopic.publish(cs);

	//ACCDAT
	accFifo.get(tempVector);
	sprintf(cs.param, "%.6s", "GYRDAT");
	vector_to_string(cs.msg, &tempVector);
	tmTopic.publish(cs);

	//ORIENT
	orientationFifo.get(tempFloat);
	sprintf(cs.param, "%.6s", "ORIENT");
	sprintf(cs.msg, "%.2f", tempFloat);
	tmTopic.publish(cs);

	//LGHTSN
	lightFifo.get(tempInt);
	sprintf(cs.param, "%.6s", "LGHTSN");
	sprintf(cs.msg, "%.2f", tempInt);
	tmTopic.publish(cs);

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

