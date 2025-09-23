#ifndef __COMMUCATE_SLAVE_H__
#define __COMMUCATE_SLAVE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gd32f10x.h"


#define PAK_HEAD_VAL				0x5A
#define PAK_TAIL_VAL				0x5B

#define PAK_RES_COM_MASK			0x80
#define PAK_RES_NO_DATA_LEN		3

#define PAK_TIMEOUT_VAL				300

#define APP_BOOT_FLAG_ADDR	0x20004FFC
#define STAY_BOOT_FLAG_VAL	0x5A5A5A5A

#define FLASH_CACHE_PAGE_SIZE		2048

typedef enum{
		PAK_HEAD,
		PAK_ADDR,
		PAK_COMMAND,
		PAK_LENGTH,
		PAK_DATA,
		PAK_CRC_H,
		PAK_CRC_L,
		PAK_TAIL,
}PackStep;

typedef enum{
		COM_FIND = 0x01,
		COM_ERASE,
		COM_WRITE,
		COM_READ,
		COM_EXIT,
		COM_ENTER_UPGRADE,
		COM_ENTER_BYPASS,
}CommandEnum;

typedef struct{
		PackStep step;
		CommandEnum command;
		uint8_t len;
		uint8_t buf[150];
		uint8_t bufLen;
		uint16_t crc;
}PakStruct;

typedef enum{
		TIMER_DISABLE_COMM,
		TIMER_ENABLE_COMM,
}TimerEnableEnum;

typedef struct{
		TimerEnableEnum TimerEnable;
		uint16_t count;
}PakTimerStr;

extern PakStruct PrtocolStr;
extern PakTimerStr PakTimer;

void ReceParse(uint8_t ch);
void PakStuctInit(void);
void CommunSlaveFun(void);
#endif
