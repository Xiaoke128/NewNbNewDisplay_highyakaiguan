#ifndef HW_FMC_H
#define HW_FMC_H

#include <stdio.h>
#include "gd32f10x.h"
#include "Init.h"

#define STORE_CONF_ADDRESS_START	0x0800FC00
#define MODBUS_DEFAULT_ADDR		0x01

#define CHIP_PAGE_SIZE				0x400

//Configure package parameters
typedef struct{
	uint16_t TemCollectInterval;//second, default 5 seconds
	uint16_t TimeTick;//minute, default random
	uint32_t CloseAlramTime;//100 Microsecond 
	uint32_t OpenAlarmTime;//100 Microsecond
	uint32_t AbnormalCloseTime;//100 Microsecond
	uint32_t AbnormalOpenTime;//100 Microsecond
	uint8_t ReportInterval;//hour, default 4 hours
	uint8_t AbnormalTem;//tempture alarm
	uint8_t TempDiffAlarmVal;//tempture different value
	uint8_t reserved[1];
}ConfStr;

typedef struct{
	uint32_t ReportStartTime;
	ConfStr ServerConf;
	uint8_t SlaveAddr;
	uint8_t reerved;
	uint32_t BaudRateModbus;
	uint32_t CrcVal;
}StoreConfStr;

extern StoreConfStr StoreConf;

void FlashEarsePage(uint32_t addr);
void FMC_StoreInfoInit(void);


#endif
