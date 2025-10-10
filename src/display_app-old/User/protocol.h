#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define PROTOCOL_HEAD			0x5A
#define PROTOCOL_TAIL			0x5B

#define NET_LED_STATUS_OFF				0x00
#define NET_LED_STATUS_FLASH_200MS		0x01
#define NET_LED_STATUS_FLASH_1000MS		0x02

#define DATA_LED_STATUS_NORMAL			0x00
#define DATA_LED_STATUS_ABNORMAL_FLASH	0x01
#define DATA_LEN_STATUS_ALARM			0x02


typedef void (*Profunc)(void);

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

typedef enum{
	COMMAND_GET_WL_STATUS = 0X01,
	COMMAND_GET_TEMP_ID,
	COMMAND_GET_HUMI_ID,
	COMMADN_GET_TEMP_VAL,
	COMMAND_GET_HUMI_VAL,
	COMMAND_SET_SELF_TEMP_HUMI,
	COMMAND_SEND_SIGNAL_INFO,
	COMMAND_SET_ABNORMAL,
	COMMAND_SEND_COMMUNICATION_STATUS,
	COMMAND_SEND_MAIN_BOARD_INFO,
}ComEnum;

typedef struct{
	ProParseStep step;
	uint8_t len;
	ComEnum command;
	uint8_t comVer;
	uint8_t data[200];
	uint8_t dataIndex;
}ProStr;

typedef struct{
	ComEnum command;
	Profunc fun;
}ComFunStr;

typedef struct{
	uint8_t enable;
	uint16_t count;
}ProTimer;

typedef struct{
	uint8_t NetLedStatus;
	uint8_t DataLedStatus;
	uint8_t slaveID;
	uint32_t baudrate;
	uint8_t IMEI[15];
	uint8_t ver[6];
	uint32_t unix;
}MainBoardInfoStr;


extern MainBoardInfoStr MainBoardInfo;

void DisplayMainProInit(void);
void DisplayMainProTask(void);

#endif
