/*
 * helper.h
 *
 *  Created on: 30.11.2014
 *      Author: holger
 */

#include "topics.h"
#include <cmath>

//void parseStringToPacket(char * str, int size, CommStruct* cs){
//	if(size < 7)
//		return;
//
//	xprintf(cs->param, "%.6s", str);
//	xprintf(cs->msg, "%.*s", size - 7, str + 6);
//}

//int parsePacketToString(char * out, CommStruct *cs, int bufLen){
//	int stringSum = PARAM_LENGTH + strlen(cs->msg);
//
//	if(bufLen < stringSum)
//		return 0;
//
//	xprintf(out, "PARAM: '%s', MSG: '%s'\0", cs->param, cs->msg);
//
//	return stringSum;
//}

void vector_cross(Vector3D *a, Vector3D*b, Vector3D *out){
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
}

float vector_dot(Vector3D *a, Vector3D *b){
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vector_normalize(Vector3D *a) {
	float mag = sqrt(vector_dot(a, a));
	a->x /= mag;
	a->y /= mag;
	a->z /= mag;
}
