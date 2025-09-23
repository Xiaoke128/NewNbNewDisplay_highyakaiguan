#ifndef ISL1208_H
#define ISL1208_H
#include "gd32f10x.h"
#include "Init.h"
#include "wh_nb73.h"
#include <stdio.h>
#include <time.h>

#define SECOND_REGISTER				0x00
#define MINUTE_REGISTER				0x01
#define HOUR_REGISTER				0x02
#define DATE_REGISTER				0x03
#define MOUTH_REGISTER				0x04
#define YEAR_REGISTER				0x05
#define DAY_REGISTER				0x06
#define STATUS_REGISTER				0x07
#define INTERRUPT_REGISTER			0x08

#define HOUR_24_FORMAT				0x80
#define ISL_INIT_STATUS				0x90

typedef struct{
	uint32_t year;
	uint8_t mouth;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}RtcTime;

void Isl1208Init(void);
void Isl1208GetTime(void);
void GetRtcTime(void);
void RtcSetTime(RtcTime time);
uint32_t UTC_to_Unix(RtcTime time);
void Unix_to_UTC(uint32_t xtime, RtcTime *time);

extern RtcTime TimeStr;
#endif
