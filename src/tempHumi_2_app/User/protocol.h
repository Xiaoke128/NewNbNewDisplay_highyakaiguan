#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef void (*Profunc)(void);

#define PRO_HRAD_VAL			0x5A
#define PRO_TAIL_VAL			0x5B

typedef enum{
	RUN_NOTHING,
	RUN_GET_WL_TEMP_STATUS,
	RUN_GET_TEMP_ID,
	RUN_GET_HUMI_ID,
	RUN_GET_TEMP_VAL,
	RUN_GET_HUMI_VAL,
	RUN_SELF_TEMP_HUMI,
	RUN_SEND_SIGNAL,
	RUN_SET_ABNORMAL,
	RUN_SEND_COMMUNICATION_STATUS,
	RUN_SEND_MAIN_BOARD_INFO,
}RunStep;

typedef union {
	uint8_t val;
	struct{
		uint8_t		WlIvild						:1;
		uint8_t		TempIDGet					:1;
		uint8_t		HumiIDGet					:1;
		uint8_t		AbnormalGet				:1;
		uint8_t		CommunicationOk		:1;
		uint8_t		Reserved					:3;
	}bit;
}ProGetFlag;

typedef union{
	uint16_t val;
	struct{
		uint8_t 	GetWlEnable		:1;
		uint8_t   GetTempIdEnable		:1;
		uint8_t   GetHumiIdEnable		:1;
		uint8_t		GetTempValEnable	:1;
		uint8_t		GetHumiValEnable	:1;
		uint8_t		SeSelfTempHumiEnable	:1;
		uint8_t 	SendSignalInfoEnable	:1;
		uint8_t		SetAbnormalEnable			:1;
		uint8_t		SendCommunicateStatus	:1;
		uint8_t		SendMainBoardInfo			:1;
		uint8_t		reserved							:6;
	}bit;
}ProEnableFlag;

typedef enum{
	COMMAND_GET_WL_STATUS = 0X01,
	COMMAND_GET_TEMP_ID,
	COMMAND_GET_HUMI_ID,
	COMMADN_GET_TEMP_VAL,
	COMMAND_GET_HUMI_VAL,
	COMMAND_SET_SELF_TEMP_HUMI,
	COMMAND_SEND_SIGNAL_INFO,
	COMMAND_SET_ABNORMAL_VAL,
	COMMAND_SEND_COMMUNICATION_STATUS,
	COMMAND_SEND_MAIN_BOARD_INFO,
}ComEnum;

typedef enum{
	PRO_HEAD,
	PRO_LEN,
	PRO_COMMAND,
	PRO_COM_VERSION,
	PRO_DATA,
	PRO_CRC_H,
	PRO_CRC_L,
	PRO_TAIL,
}ProParseStep;

typedef struct{
	ProParseStep step;
	uint8_t len;
	uint8_t command;
	uint8_t buf[200];
	uint8_t bufIndex;
	uint8_t ParseDone;
}ProtocolStr;

typedef struct{
	RunStep runStep;
	ProGetFlag getFlag;
	ProEnableFlag enableFlag;
}ProFunStr;

void ParsePro(uint8_t ch);
void ProtocolTask(void);
void SendMainBoardInfoEnable(void);

#endif
