#ifndef __WIRELESS_TEMP_H__
#define __WIRELESS_TEMP_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef union {
	uint8_t val;
	struct{
		uint8_t		Connected					:1;
		uint8_t		TempIDGet					:1;
		uint8_t		HumiIDGet					:1;
		uint8_t		Reserved					:5;
	}bit;
}WlSysFlag;

typedef union {
	uint8_t val;
	struct{
		uint8_t		getConnect				:1;
		uint8_t		getTempID				:1;
		uint8_t		getTempVal				:1;
		uint8_t 	getHumiID				:1;
		uint8_t 	getHumiVal				:1;
		uint8_t		Reserved				:3;
	}bit;
}Wlflag;

typedef enum{
		WL_RUNNING_NOTHING,
		WL_RUNNING_CHECK_CONNECT,
		WL_RUNNING_GET_TEMP_ID,
		WL_RUNNING_GET_TEMP_VAL,
		WL_RUNNING_GET_HUMI_ID,
		WL_RUNNING_GET_HUMI_VAL,
}wlRun;

typedef struct{
	WlSysFlag sysFlag;
	wlRun	runStep;
	Wlflag flag;
	uint16_t tempID_Val[90][2];
	uint16_t humiID_Val[15][2];
}WlStr;

extern WlStr wlStrData;

void WlVarInit(void);
void GetWirelessTempTask(void);

#endif
