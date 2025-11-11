#ifndef __MT29F2G01ABAGDWB_H__
#define __MT29F2G01ABAGDWB_H__

#include <stdio.h>
#include "gd32f10x.h"

#define FLASH_ID							0x00C22011

#define FALSH_BLOCK_SIZE					(64 * 1024)
#define FLASH_BLOCK0_ADDR					0x00000000u
#define FLASH_BLOCK1_ADDR					0x00010000u

//#define FLASH_PAGE_SIZE				2048
//#define WRITE_CACHE_ADDR_MASK	0xE000
//#define USE_BLOCK_ADDR				0x000080//block 2
//#define FILE_LENGTH_PAGE			USE_BLOCK_ADDR
//#define FILE_STORE_START_PAGE	(FILE_LENGTH_PAGE + 1)

#define COMMAND_READ_ID				0x9F
//#define COMMAND_GET_FEATURE			0x0F
//#define COMMAND_SET_FEATURE			0x1F
#define COMMAND_WRITE_ENABLE		0x06
#define COMMAND_WRITE_DISABLE		0x04
#define COMMAND_EARSE_SECTOR		0x20
#define COMMAND_EARSE_BLOCK			0xD8
#define COMMAND_FAST_READ_DATA		0x03//0x0B
#define COMMAND_PROGRAM_DATA		0x02
#define COMMAND_READ_STATUS			0x05

//#define COMMAND_PROGRAM_LOAD		0x02
//#define COMMAND_PROGRAM_EXEU		0x10
//#define COMMAND_READ_PAGE_TO_CACHE	0x13
//#define COMMAND_READ_FROM_CACHE		0x03


//#define REG_BLOCK_LOCK				0xA0
//#define REG_STATUS						0xC0

//#define DISABLE_WP_REG_VAL		0x00


#define STATUS_MASK_VAL				0x01

void NandFlashInit(void);
void FlashFunTest(void);
void EarseBlock(uint8_t blockNum);
void ReadDatFromFlash(uint32_t addr, uint8_t *buf, uint16_t len);
void WriteDataToFlash(uint32_t addr, uint8_t *buf, uint16_t len);
#endif

