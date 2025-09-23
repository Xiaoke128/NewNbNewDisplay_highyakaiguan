#ifndef __GUI_H__
#define __GUI_H__
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include "Init.h"
#include <time.h>

typedef struct{
	uint32_t year;
	uint8_t mouth;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}RtcTime;

#define OLED_SCROING_COUNT		30

void MainMenuFun(void);
void DatalogMenuFun(void);
void InterlayerHumidityFun(void);
void ConactTempSecondFun(void);
void ContactTempFirstFun(void);
void MechProperitesFun(void);
void DeviceInitFun(void);
void DeviceStartFun(void);
#endif

