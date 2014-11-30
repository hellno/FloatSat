/*
 * helper.h
 *
 *  Created on: 30.11.2014
 *      Author: holger
 */

#include "topics.h"

void parseStringToPacket(char * str, int size, CommStruct* cs){
	if(size < 7)
		return;

	sprintf(cs->param, "%.6s", str);
	sprintf(cs->msg, "%.*s", size - 7, str + 6);
}

int parsePacketToString(char * out, CommStruct *cs, int bufLen){
	int stringSum = PARAM_LENGTH + strlen(cs->msg);

	if(bufLen < stringSum)
		return 0;

	sprintf(out, "PARAM: '%s', MSG: '%s'\0", cs->param, cs->msg);

	return stringSum;
}
