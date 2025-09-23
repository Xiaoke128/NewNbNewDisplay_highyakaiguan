#ifndef __OLED_H__
#define __OLED_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gd32f10x.h"
#include "systick.h"

#define OLED_RESET_LOW		GPIO_BC(GPIOA) = GPIO_PIN_9
#define OLED_RESET_HIGH		GPIO_BOP(GPIOA) = GPIO_PIN_9

#define OLED_DATA_REG			GPIO_BOP(GPIOA) = GPIO_PIN_10
#define OLED_COMMAND_REG	GPIO_BC(GPIOA) = GPIO_PIN_10

#define OLED_POWER_ON			GPIO_BOP(GPIOA) = GPIO_PIN_11
#define OLED_POWER_OFF		GPIO_BC(GPIOA) = GPIO_PIN_11

typedef struct{
	uint8_t currentInterTemp;
	uint8_t currentCj2301Temp;
	uint8_t currentCj2301Humi;
	uint32_t currentInterTempUnix;
	uint8_t Cj2301Temp[30];
	uint8_t Cj2301Humi[30];
	uint32_t InterTempUnix[30];
	uint8_t InterTempIndex;
	int8_t currentTemp[6];
	uint32_t currentTempUnix[6];
	int8_t selfTemp[6][30];	
	uint32_t selfTempUnix[6][30];
	uint8_t selfTempIndex[6][1];
	uint8_t currentHumi;
	uint32_t currentHumiUnix;
	uint8_t selfHumi[30];
	uint32_t selfHumiUnix[30];
	uint8_t selfHumiIndex;
	uint16_t signalTime[2][30];
	uint32_t signalUnix[2][30];
	uint8_t signalIndex[2][1];
	int8_t abnormalTempVal;
	uint8_t abnormalHumiVal;
	uint16_t SignalOpenAbnormalVal;
	uint16_t SignalCloseAbnormalVal;
	uint8_t abnormalHumi[30];
	uint32_t abnormalHumiUnix[30];
	uint8_t abnormalHumiIndex;
	int8_t abnormalTemp[30];
	uint32_t abnormalTempUnix[30];
	uint8_t abnormalTempIndex;
	uint16_t abnormnalSignal[30];
	uint8_t abnormalSignalType[30];
	uint32_t abnormnalSignalUnix[30];
	uint8_t abnormnalSignalIndex;
}DispalyStoreData;

extern DispalyStoreData DisplayData;

void DisplayInterTempAdd(uint8_t tempVal, uint8_t humiVal, uint32_t unix);
void DispalyTempStoreAdd(uint8_t tempNum, int8_t tempVal, uint32_t unix);
void DispalyHumiStoreAdd(uint8_t humiVal, uint32_t unix);
void DisplaySignalStoreAdd(uint8_t type, uint16_t time, uint32_t unix);
//void DisalyAbnormalInterTempAdd(uint8_t tempVal, uint32_t unix);
void DisalyAbnormalHumiAdd(uint8_t humiVal, uint32_t unix);
void DisplayAnnormalTempAdd(uint8_t tempNum, int8_t tempVal, uint32_t unix);
void DisplayAbnormalSignalAdd(uint8_t type, uint16_t time, uint32_t unix);

void Fill_RAM(unsigned char Data);
void OledInit(void);
void ShowChinese(const uint8_t *Data_Pointer, uint8_t StartPage, uint8_t StartColum, uint8_t NumHanzi, uint8_t mode);
void ShowChar(uint8_t *ShowStr, uint8_t NumChar, uint8_t StartPage, uint8_t StartColum, uint8_t mode);
void MechProShowScrolling(uint8_t dataSrc[2][280], uint16_t numColun, uint8_t startPage, uint16_t index);
void DisplayOnePage(unsigned char a, unsigned char b);
void Write8_8_char(uint8_t data, uint8_t page, uint8_t colum, uint8_t num);
void Frame(void);
#endif
