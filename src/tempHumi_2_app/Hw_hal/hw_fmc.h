#ifndef HW_FMC_H
#define HW_FMC_H

#include <stdio.h>
#include "gd32f10x.h"
#include "Init.h"

#define CHIP_PAGE_SIZE				0x400

#define STORE_INFO_VERSION_1		0x01
#define STORE_INFO_VERSION_2		0x02
#define STORE_INFO_VERSION_3		0x03
#define STORE_INFO_VERSION_4		0x04

#define STORE_INFO_ADDRESS_START	0x0800F800
#define STORE_CONF_ADDRESS_START	0x0800FC00

#define STORE_INTERVAL_5_MINUTES		(5 * 60 * 1000)

uint32_t CheckCRC(uint8_t *buf, uint16_t len);
void WriteStoreConf(void);
void WriteStoreInfo(void);
void FMC_StoreInfoInit(void);
void StoreInfoAdd(void);
void StoreInfoCheck(void);
void StoreConfUpdate(void);
void StoreConfStore(void);
#endif
