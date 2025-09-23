#ifndef __MT29F2G01ABAGDWB_H__
#define __MT29F2G01ABAGDWB_H__

#include <stdio.h>
#include "gd32f10x.h"

#define FLASH_ID							0x2C24

#define FLASH_PAGE_SIZE				2048
#define WRITE_CACHE_ADDR_MASK	0xE000
#define USE_BLOCK_ADDR				0x000080//block 2
#define FILE_LENGTH_PAGE			USE_BLOCK_ADDR
#define FILE_STORE_START_PAGE	(FILE_LENGTH_PAGE + 1)

#define COMMAND_READ_ID				0x9F
#define COMMAND_GET_FEATURE		0x0F
#define COMMAND_SET_FEATURE		0x1F
#define COMMAND_WRITE_ENABLE	0x06
#define COMMAND_WRITE_DISABLE	0x04
#define COMMAND_EARSE_BLOCK		0xD8
#define COMMAND_PROGRAM_LOAD	0x02
#define COMMAND_PROGRAM_EXEU	0x10
#define COMMAND_READ_PAGE_TO_CACHE	0x13
#define COMMAND_READ_FROM_CACHE			0x03


#define REG_BLOCK_LOCK				0xA0
#define REG_STATUS						0xC0

#define DISABLE_WP_REG_VAL		0x00


#define STATUS_MASK_VAL				0x01

void NandFlashInit(void);
void FlashFunTest(void);
void EarseBlock(void);
void WritePageLoad(uint8_t *buf, uint16_t len, uint16_t addr);
void WritePageExe(uint32_t addr);
void ReadPageToCache(uint32_t addr);
void ReadFromCache(uint8_t *buf, uint16_t len, uint16_t addr);
#endif

