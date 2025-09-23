#include "GUI.h"
pFunction GUI_Func = NULL;
static uint8_t logSelectNum = 0;
static uint8_t abnormalSelectNum = 0;
static void AbnormalMenuFun(void);

const uint8_t MainMenu[][16] = {
{0x00,0x08,0x08,0x08,0x08,0x08,0x09,0xFA,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x7F,0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x00},/*"主",0*/
{0x04,0x04,0x44,0xC4,0x4F,0x44,0x44,0xC4,0x24,0x24,0x2F,0xB4,0x24,0x04,0x04,0x00},
{0x40,0x44,0x24,0x24,0x15,0x0C,0x04,0xFE,0x04,0x0C,0x15,0x24,0x24,0x44,0x40,0x00},/*"菜",1*/
{0x00,0x00,0xF8,0x49,0x4A,0x4C,0x48,0xF8,0x48,0x4C,0x4A,0x49,0xF8,0x00,0x00,0x00},
{0x10,0x10,0x13,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x13,0x10,0x10,0x00},/*"单",2*/
};

const uint8_t DataShow[][16] = {
{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"1",0*/
{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},/*".",1*/
{0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00},/*"数",2*/
{0x10,0x10,0xFF,0x10,0x90,0x00,0xFE,0x92,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00},
{0x42,0x82,0x7F,0x01,0x80,0x60,0x1F,0x00,0xFC,0x44,0x47,0x44,0x44,0xFC,0x00,0x00},/*"据",3*/
{0x00,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,0x00},
{0x40,0x42,0x44,0x58,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x50,0x48,0x46,0x40,0x00},/*"显",4*/
{0x40,0x40,0x42,0x42,0x42,0x42,0x42,0xC2,0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00},
{0x20,0x10,0x08,0x06,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x02,0x04,0x08,0x30,0x00},/*"示",5*/
};

const uint8_t dataLog[][16] = {
{0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"2",0*/
{0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},/*".",1*/
{0x00,0x00,0xFE,0x02,0x42,0x42,0x42,0x42,0xFA,0x42,0x42,0x42,0x42,0xC2,0x02,0x00},
{0x80,0x60,0x1F,0x80,0x40,0x20,0x18,0x06,0x01,0x00,0x40,0x80,0x40,0x3F,0x00,0x00},/*"历",2*/
{0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,0x88,0xF8,0x00,0x00,0x00},
{0x80,0x80,0x81,0x44,0x48,0x30,0x10,0x2F,0x20,0x40,0x40,0x40,0x81,0x80,0x80,0x00},/*"史",3*/
{0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00},/*"数",4*/
{0x10,0x10,0xFF,0x10,0x90,0x00,0xFE,0x92,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00},
{0x42,0x82,0x7F,0x01,0x80,0x60,0x1F,0x00,0xFC,0x44,0x47,0x44,0x44,0xFC,0x00,0x00},/*"据",5*/

};

const uint8_t AbnrmalData[][16] = {
	
{0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"3",0*/
{0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},/*".",1*/
{0x00,0x00,0x7E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0xE0,0x00,0x00},
{0x08,0x88,0x48,0x28,0x1E,0x08,0x08,0x08,0x08,0x08,0xFE,0x08,0x08,0x08,0x08,0x00},/*"异",2*/
{0x20,0x18,0x08,0xEA,0xAC,0xA8,0xA8,0xAF,0xA8,0xA8,0xAC,0xEA,0x08,0x28,0x18,0x00},
{0x00,0x00,0x3E,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x12,0x22,0x1E,0x00,0x00,0x00},/*"常",3*/
{0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00},
{0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00},/*"数",4*/
{0x10,0x10,0xFF,0x10,0x90,0x00,0xFE,0x92,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00},
{0x42,0x82,0x7F,0x01,0x80,0x60,0x1F,0x00,0xFC,0x44,0x47,0x44,0x44,0xFC,0x00,0x00},/*"据",5*/
};

const uint8_t AsciiIndex[] = "0123456789ms:/\\.%*() MVJC-IEDABFGHKLNOPQRSTUWXYZ";

const uint8_t AsciiCode[][8] = {
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},/*"0",0*/
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"1",1*/
	{0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},/*"2",2*/
	{0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00},
	{0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00},/*"3",3*/
	{0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00},
	{0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00},/*"4",4*/
	{0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00},
	{0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00},/*"5",5*/
	{0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00},
	{0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00},/*"6",6*/
	{0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00},/*"7",7*/
	{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00},
	{0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},/*"8",8*/
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00},
	{0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00},/*"9",9*/
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},/*"m",10*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00},
	{0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},/*"s",11*/
	{0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00},/*":",12*/
	{0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04},
	{0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00},/*"/",13*/
	{0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00},/*"\",14*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00},/*".",15*/
	{0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00},
	{0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00},/*"%",16*/
	{0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00},
	{0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00},/*"*",17*/
	{0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00},
	{0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00},/*"(",18*/
	{0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00},
	{0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00},/*")",19*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",20*/
	{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00},
	{0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00},/*"M",21*/
	{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08},
	{0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},/*"V",22*/
	{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00},
	{0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},/*"J",23*/
	{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00},
	{0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},/*"C",24*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01},/*"-",25*/
	{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00},
	{0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"I",0*/
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},/*"E",1*/
	{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},/*"D",2*/
	{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00},
	{0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},/*"A",0*/
	{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},/*"B",1*/
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},/*"F",2*/
	{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00},
	{0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},/*"G",3*/
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},/*"H",4*/
	{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00},
	{0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},/*"K",5*/
	{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00},
	{0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},/*"L",6*/
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08},
	{0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},/*"N",7*/
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},/*"O",8*/
	{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00},
	{0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},/*"P",9*/
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00},
	{0x0F,0x10,0x28,0x28,0x30,0x50,0x4F,0x00},/*"Q",10*/
	{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00},
	{0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},/*"R",11*/
	{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00},
	{0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},/*"S",12*/
	{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},/*"T",13*/
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08},
	{0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},/*"U",14*/
	{0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00},
	{0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00},/*"W",15*/
	{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08},
	{0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},/*"X",16*/
	{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00},
	{0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},/*"Y",17*/
	{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00},
	{0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},/*"Z",18*/
};

const uint8_t MechPeoCode[][16] = {
	{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00},
	{0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x00,0x00,0x00,0x3F,0x40,0x40,0x78,0x00},/*"机",0*/
	{0x10,0x10,0xD0,0xFF,0x90,0x00,0x10,0xD0,0x10,0xD0,0x10,0xFF,0x10,0x92,0x14,0x00},
	{0x04,0x03,0x00,0xFF,0x00,0x41,0x31,0x0F,0x01,0xBF,0x41,0x27,0x38,0x47,0xF0,0x00},/*"械",1*/
	{0x40,0x3C,0x10,0xFF,0x10,0x10,0x40,0x48,0x48,0x48,0x7F,0x48,0xC8,0x48,0x40,0x00},
	{0x02,0x06,0x02,0xFF,0x01,0x01,0x00,0x02,0x0A,0x12,0x42,0x82,0x7F,0x02,0x02,0x00},/*"特",2*/
	{0x00,0xE0,0x00,0xFF,0x10,0x20,0x40,0x3C,0x10,0x10,0xFF,0x10,0x10,0x10,0x00,0x00},
	{0x01,0x00,0x00,0xFF,0x00,0x00,0x40,0x42,0x42,0x42,0x7F,0x42,0x42,0x42,0x40,0x00},/*"性",3*/
};

const uint8_t TempCode[][16] = {
	{0x06,0x09,0x09,0xE6,0xF8,0x0C,0x04,0x02,0x02,0x02,0x02,0x02,0x04,0x1E,0x00,0x00},
	{0x00,0x00,0x00,0x07,0x1F,0x30,0x20,0x40,0x40,0x40,0x40,0x40,0x20,0x10,0x00,0x00},/*"℃",0*/
};

const uint8_t CloseCode[][16] = {
	{0x40,0x40,0x20,0x20,0x50,0x48,0x44,0x43,0x44,0x48,0x50,0x20,0x20,0x40,0x40,0x00},
	{0x00,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0xFE,0x00,0x00,0x00,0x00},/*"合",0*/
	{0x00,0xF8,0x01,0x02,0xF0,0x92,0x92,0xF2,0x92,0x92,0xF2,0x02,0x02,0xFE,0x00,0x00},
	{0x00,0xFF,0x00,0x00,0x0F,0x04,0x04,0x7F,0x04,0x04,0x0F,0x40,0x80,0x7F,0x00,0x00},/*"闸",1*/
};

const uint8_t OpenCode[][16] = {
	{0x80,0x40,0x20,0x90,0x88,0x86,0x80,0x80,0x80,0x83,0x8C,0x10,0x20,0x40,0x80,0x00},
	{0x00,0x80,0x40,0x20,0x18,0x07,0x00,0x40,0x80,0x40,0x3F,0x00,0x00,0x00,0x00,0x00},/*"分",0*/
	{0x00,0xF8,0x01,0x02,0xF0,0x92,0x92,0xF2,0x92,0x92,0xF2,0x02,0x02,0xFE,0x00,0x00},
	{0x00,0xFF,0x00,0x00,0x0F,0x04,0x04,0x7F,0x04,0x04,0x0F,0x40,0x80,0x7F,0x00,0x00},/*"闸",1*/
};

const uint8_t InterTemCode[][16] = {
	{0x00,0xF8,0x08,0x08,0x08,0x08,0x88,0x7F,0x88,0x08,0x08,0x08,0x08,0xF8,0x00,0x00},
	{0x00,0xFF,0x00,0x08,0x04,0x02,0x01,0x00,0x00,0x01,0x02,0x4C,0x80,0x7F,0x00,0x00},/*"内",0*/
	{0x40,0x44,0x54,0x65,0x46,0x44,0x64,0x54,0x44,0x40,0xFE,0x02,0x22,0xDA,0x06,0x00},
	{0x00,0x00,0x7E,0x22,0x22,0x22,0x22,0x7E,0x00,0x00,0xFF,0x08,0x10,0x08,0x07,0x00},/*"部",1*/
	{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
	{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"温",2*/
	{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
	{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",3*/
};
const uint8_t TempHumiCode[][16] = {
{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"温",0*/
{0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00},/*"湿",1*/
{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",2*/
};

const uint8_t DeviceInitCode[][16] = {
	{0x00,0x00,0x00,0xF8,0x88,0x8C,0x8A,0x89,0x88,0x88,0x88,0xF8,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00,0x00},/*"自",0*/
	{0x10,0x10,0xD0,0xFF,0x90,0x50,0x20,0x50,0x4C,0x43,0x4C,0x50,0x20,0x40,0x40,0x00},
	{0x04,0x03,0x00,0xFF,0x00,0x41,0x44,0x58,0x41,0x4E,0x60,0x58,0x47,0x40,0x40,0x00},/*"检",1*/
	{0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
	{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},/*"中",2*/
};

const uint8_t ProcessBarCode[][8] = {
	{0x00,0xF8,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x88,0x88,0x88,0x88,0x88,0x88,0xF8,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

const uint8_t ContactTempCode[][16] = {
	{0x10,0xEC,0x27,0xF4,0x2C,0xE0,0x00,0xF8,0x08,0x08,0xFF,0x08,0x08,0xF8,0x00,0x00},
	{0x80,0x7F,0x09,0x3F,0x89,0xFF,0x20,0x63,0x21,0x21,0x3F,0x21,0x29,0x33,0x60,0x00},/*"触",0*/
	{0x00,0x00,0x10,0x60,0x04,0x18,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x81,0x81,0x41,0x41,0x21,0x11,0x09,0x07,0x01,0x05,0x09,0x11,0x21,0xC1,0x01,0x00},/*"头",1*/
	{0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
	{0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00},/*"温",2*/
	{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
	{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",3*/
};

const uint8_t InterlayerHumiCode[][16] = {
	{0x00,0x08,0x08,0x28,0x48,0x08,0x08,0xFF,0x08,0x08,0x48,0x28,0x08,0x08,0x00,0x00},
	{0x81,0x81,0x41,0x41,0x21,0x11,0x0D,0x03,0x0D,0x11,0x21,0x41,0x41,0x81,0x81,0x00},/*"夹",0*/
	{0x00,0x00,0xFE,0x12,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x9E,0x00,0x00},
	{0x80,0x60,0x1F,0x04,0x44,0xE4,0x54,0x4C,0x44,0x44,0x44,0x54,0x64,0xC4,0x04,0x00},/*"层",1*/
	{0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
	{0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00},/*"湿",2*/
	{0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00},
	{0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00},/*"度",3*/
};

const uint8_t upDownDirCode[][16] = {
	{0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x06,0x18,0x60,0x80,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x20,0x18,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x18,0x20,0x00,0x00},/*"∧",0*/
	{0x00,0x04,0x18,0x60,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x01,0x06,0x18,0x60,0x18,0x06,0x01,0x00,0x00,0x00,0x00,0x00},/*"∨",1*/
};
const uint8_t NothingCode[][16] = {
{0x00,0x40,0x42,0x42,0x42,0xC2,0x7E,0x42,0xC2,0x42,0x42,0x42,0x40,0x40,0x00,0x00},
{0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x70,0x00,0x00},/*"无",0*/
};

const uint8_t CommuncationFailCode[][16] = {
{0x00,0x08,0x08,0x08,0x08,0x08,0x09,0xFA,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00},
{0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x7F,0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x00},/*"主",0*/
{0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFC,0x24,0xE4,0x24,0x22,0x23,0xE2,0x00,0x00},
{0x04,0x03,0x00,0xFF,0x00,0x83,0x60,0x1F,0x80,0x41,0x26,0x18,0x26,0x41,0x80,0x00},/*"板",1*/
{0x40,0x42,0xCC,0x00,0x00,0xE2,0x22,0x2A,0x2A,0xF2,0x2A,0x26,0x22,0xE0,0x00,0x00},
{0x80,0x40,0x3F,0x40,0x80,0xFF,0x89,0x89,0x89,0xBF,0x89,0xA9,0xC9,0xBF,0x80,0x00},/*"通",2*/
{0x00,0x80,0x60,0xF8,0x07,0x00,0x04,0x24,0x24,0x25,0x26,0x24,0x24,0x24,0x04,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x00,0x00,0xF9,0x49,0x49,0x49,0x49,0x49,0xF9,0x00,0x00},/*"信",3*/
{0x00,0x00,0x7E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0xE0,0x00,0x00},
{0x08,0x88,0x48,0x28,0x1E,0x08,0x08,0x08,0x08,0x08,0xFE,0x08,0x08,0x08,0x08,0x00},/*"异",4*/
{0x20,0x18,0x08,0xEA,0xAC,0xA8,0xA8,0xAF,0xA8,0xA8,0xAC,0xEA,0x08,0x28,0x18,0x00},
{0x00,0x00,0x3E,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x12,0x22,0x1E,0x00,0x00,0x00},/*"常",5*/
};
const uint8_t ContinusCode[][16] = {
{0x20,0x30,0xAC,0x63,0x10,0x00,0xFE,0x48,0x50,0x40,0xFF,0x40,0x50,0x48,0x00,0x00},
{0x22,0x67,0x22,0x12,0x12,0x00,0x7F,0x44,0x42,0x41,0x7F,0x41,0x42,0x44,0x40,0x00},/*"继",0*/
{0x20,0x30,0xAC,0x63,0x20,0x18,0x20,0x24,0xA4,0x24,0x3F,0xA4,0x24,0xA4,0x60,0x00},
{0x22,0x67,0x22,0x12,0x12,0x12,0x88,0x8A,0x4C,0x29,0x18,0x0F,0x18,0x28,0xC8,0x00},/*"续",1*/
};

static void Unix_to_UTC(uint32_t xtime, RtcTime *time)
{
	struct tm *timeTemp;
	
	xtime += 8 * 60 * 60;//benjing time need add 8 hours
	timeTemp = localtime(&xtime);
	time->year = timeTemp->tm_year + 1900;
	time->mouth = timeTemp->tm_mon + 1;//0 ~ 11 need add 1
	time->date = timeTemp->tm_mday;
	time->hour = timeTemp->tm_hour;
	time->minute = timeTemp->tm_min;
	time->second = timeTemp->tm_sec;
	
}

uint16_t GetMainBoardInfo(uint8_t dataDest[2][280], uint8_t page)
{
		char buf[30] = {0};
		uint8_t i = 0, j = 0, k = 0;
		RtcTime RtcTime;
		
		if(page == 0)
		{
				sprintf(buf, "IMEI:");
				memcpy(&buf[5], MainBoardInfo.IMEI, 15);
				buf[20] = ' ';
				for( i = 0; i < strlen(buf); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(buf[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else if(page == 2)
		{
				sprintf(buf, "ID:%d %d", MainBoardInfo.slaveID, MainBoardInfo.baudrate);
				for( i = 0; i < strlen(buf); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(buf[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else if(page == 4)
		{
				Unix_to_UTC(MainBoardInfo.unix, &RtcTime);
				sprintf(buf, "%d/%d/%d %d:%d:%d ",RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				for( i = 0; i < strlen(buf); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(buf[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else
		{
				//sprintf(buf, "     ");
				memcpy(buf, MainBoardInfo.ver, 6);
				for( i = 0; i < strlen(buf); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(buf[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "    %d", DisplayData.currentInterTemp);
				for( i = 0; i < strlen(buf); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(buf[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				memcpy(&dataDest[0][k], &TempCode[0][0], 16);
				memcpy(&dataDest[1][k], &TempCode[1][0], 16);
				k += 16;
				return k;
		}
}

void MainBoardInfoFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
		uint8_t Code1[2][280] = {0};
		uint8_t Code2[2][280] = {0};
		uint8_t Code3[2][280] = {0};
		uint8_t Code4[2][280] = {0};
		uint16_t CodeLineNum1 = 0;
		uint16_t CodeLineNum2 = 0;
		uint16_t CodeLineNum3 = 0;
		uint16_t CodeLineNum4 = 0;
		uint16_t maxLine = 0;
		static uint16_t Index1 = 0, Index2 = 0, Index3 = 0, Index4 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Index1 = 0;
				Index2 = 0;
				Index3 = 0;
				delayFlag = FALSE;
				countDelay = 0; 
				count = 0;
				Fill_RAM(0x00);
				CodeLineNum1 = GetMainBoardInfo(Code1, 0);
				MechProShowScrolling(Code1, CodeLineNum1, 0, Index1);
				CodeLineNum2 = GetMainBoardInfo(Code2, 2);
				MechProShowScrolling(Code2, CodeLineNum2, 2, Index2);
				CodeLineNum3 = GetMainBoardInfo(Code3, 4);
				MechProShowScrolling(Code3, CodeLineNum3, 4, Index3);
				CodeLineNum4 = GetMainBoardInfo(Code4, 6);
				MechProShowScrolling(Code4, CodeLineNum4, 6, Index4);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						
					break;
					case KEY_DOWN_CLICK:
						
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								Index2++;			
								Index3++;	
								memset(Code1, 0, sizeof(Code1));
								memset(Code2, 0, sizeof(Code2));
								memset(Code3, 0, sizeof(Code3));
								memset(Code4, 0, sizeof(Code4));
								CodeLineNum1 = GetMainBoardInfo(Code1, 0);
								CodeLineNum2 = GetMainBoardInfo(Code2, 2);
								CodeLineNum3 = GetMainBoardInfo(Code3, 4);
								CodeLineNum4 = GetMainBoardInfo(Code4, 6);
								maxLine = CodeLineNum1 >= CodeLineNum3 ? CodeLineNum1 : CodeLineNum3;
								CodeLineNum1 = CodeLineNum1 > 128 ? maxLine : CodeLineNum1;
								CodeLineNum3 = CodeLineNum3 > 128 ? maxLine : CodeLineNum3;
								if(Index1 >= CodeLineNum1)
								{	
										Index1 = 0;
								}
											
								if(Index2 >= CodeLineNum2)
								{
										Index2 = 0;
								}													
								if(Index3 >= CodeLineNum3)
								{
										Index3 = 0;
								}
								if(Index4 >= CodeLineNum4)
								{
										Index4 = 0;
								}
								MechProShowScrolling(Code1, CodeLineNum1, 0, Index1);
								MechProShowScrolling(Code2, CodeLineNum2, 2, Index2);
								MechProShowScrolling(Code3, CodeLineNum3, 4, Index3);
								MechProShowScrolling(Code4, CodeLineNum4, 6, Index4);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
			default:
				
			break;
		}
}

//selectLine
//0:main manu
//1:data show
//2:data log
//3:abnormal data
void GUI_MainMenu(uint8_t selectLine)
{
		uint8_t tempPage = selectLine * 2;
		ShowChinese(&MainMenu[0][0], 0x00, 40, 0x03, 0x00);
		if(tempPage == 0x02)
		{
				DisplayOnePage(0x02, 0x01);
				ShowChinese(&DataShow[0][0], 0x02, 0x18, 0x05, 0x01);
				DisplayOnePage(0x04, 0x00);
				ShowChinese(&dataLog[0][0], 0x04, 0x18, 0x05, 0x00);
				DisplayOnePage(0x06, 0x00);
				ShowChinese(&AbnrmalData[0][0], 0x06, 0x18, 0x05, 0x00);
		}
		else if(tempPage == 0x04)
		{
				DisplayOnePage(0x02, 0x00);
				ShowChinese(&DataShow[0][0], 0x02, 0x18, 0x05, 0x00);
				DisplayOnePage(0x04, 0x01);				
				ShowChinese(&dataLog[0][0], 0x04, 0x18, 0x05, 0x01);
				DisplayOnePage(0x06, 0x00);	
				ShowChinese(&AbnrmalData[0][0], 0x06, 0x18, 0x05, 0x00);
		}
		else
		{
				DisplayOnePage(0x02, 0x00);	
				ShowChinese(&DataShow[0][0], 0x02, 0x18, 0x05, 0x00);
				DisplayOnePage(0x04, 0x00);	
				ShowChinese(&dataLog[0][0], 0x04, 0x18, 0x05, 0x00);
				DisplayOnePage(0x06, 0x01);								
				ShowChinese(&AbnrmalData[0][0], 0x06, 0x18, 0x05, 0x01);
		}
}


void MainMenuFun(void)
{
		static uint8_t step = 0;
		static uint8_t SelectLine = 1;
		KeyVal key = KEY_NONE;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Fill_RAM(0x00);
				GUI_MainMenu(SelectLine);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						if(SelectLine > 1)
						{
							SelectLine--;
							
						}
						else
						{
							SelectLine = 3;
						}
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						if(SelectLine < 3)
						{
							SelectLine++;
							
						}
						else
						{
							SelectLine = 1;
						}
						step = 0;
					break;
					case KEY_BACK_CLICK:
						SelectLine = 1;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						if(SelectLine == 1)
						{
								GUI_Func = MechProperitesFun;
						}
						else if(SelectLine == 2)
						{
								GUI_Func = DatalogMenuFun;
						}
						else
						{
								GUI_Func = AbnormalMenuFun;
						}
						step = 0;
						SelectLine = 1;
					break;
					case KEY_NONE:
						
					break;
					case KEY_UP_LONG:
						
					break;
					case KEY_DOWN_LONG:
						
					break;
					case KEY_BACK_LONG:
						
					break;
					case KEY_ENETR_LONG:
						GUI_Func = MainBoardInfoFun;
						step = 0;
						SelectLine = 1;
					break;
					default:
						
					break;
				}
			break;
			default:
				
			break;
		}
}

//static uint8_t MechProDataCloseCode[2][250] = {0};
//static uint8_t MechProDataOpenCode[2][250] = {0};
//static uint8_t MechProDataInterTemCode[2][250] = {0};
//static uint8_t MechProDataCloseCodeLen = 0;
//static uint8_t MechProDataOpenCodeLen = 0;
//static uint8_t MechProDataInterTemCodeLen = 0;

static uint16_t GetMechProDataCloseCode(uint8_t dataDest[2][280])
{
		uint8_t i = 0, j = 0;
		uint8_t index = 0;
		//uint8_t buf2[29] = ":****.*ms 21/09/11 13/20/05 ";
		uint16_t k = 0;
		char TimeStr[30] = {0};
		float TimeFloat = 0.0;
		RtcTime RtcTime;
		//
		memcpy(&dataDest[0][0], &CloseCode[0][0], 16);
		memcpy(&dataDest[1][0], &CloseCode[1][0], 16);
		memcpy(&dataDest[0][16], &CloseCode[2][0], 16);
		memcpy(&dataDest[1][16], &CloseCode[3][0], 16);
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if(':' == AsciiIndex[i])
				{
						memcpy(&dataDest[0][32], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][32], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		if(DisplayData.signalIndex[0][0] == 0)//沒有數據
		{
				memcpy(&dataDest[0][40], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][40], &NothingCode[1][0], 16);
				return 56;
		}
		for(i = 0; i < DisplayData.signalIndex[0][0]; i++)
		{
				if(DisplayData.signalTime[0][i] > 10000)
				{
						index++;
				}
				else
				{
						break;
				}
		}
		if(index >= DisplayData.signalIndex[0][0])
		{
				memcpy(&dataDest[0][40], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][40], &NothingCode[1][0], 16);
				return 56;
		}
		TimeFloat = (float)(DisplayData.signalTime[0][index]) / 10;
		Unix_to_UTC(DisplayData.signalUnix[0][index], &RtcTime);
		sprintf(TimeStr, "%0.1fms %d/%d/%d %d:%d:%d ", TimeFloat, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
		k = 40;
		for( i = 0; i < strlen(TimeStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(TimeStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
#if 0
		for(i = 0; i < (sizeof(buf2) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf2[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
#endif
		return k;
}

static uint16_t GetMechProDataOpenCode(uint8_t dataDest[2][280])
{
		uint8_t i = 0, j = 0;
		uint8_t index = 0;
		uint16_t k = 0;
		char TimeStr[30] = {0};
		float TimeFloat = 0.0;
		RtcTime RtcTime;
		//
		memcpy(&dataDest[0][0], &OpenCode[0][0], 16);
		memcpy(&dataDest[1][0], &OpenCode[1][0], 16);
		memcpy(&dataDest[0][16], &OpenCode[2][0], 16);
		memcpy(&dataDest[1][16], &OpenCode[3][0], 16);
	
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if(':' == AsciiIndex[i])
				{
						memcpy(&dataDest[0][32], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][32], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		if(DisplayData.signalIndex[1][0] == 0)//沒有數據
		{
				memcpy(&dataDest[0][40], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][40], &NothingCode[1][0], 16);
				return 56;
		}
		for(i = 0; i < DisplayData.signalIndex[1][0]; i++)
		{
				if(DisplayData.signalTime[1][i] > 10000)
				{
						index++;
				}
				else
				{
						break;
				}
		}
		if(index >= DisplayData.signalIndex[1][0])
		{
				memcpy(&dataDest[0][40], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][40], &NothingCode[1][0], 16);
				return 56;
		}
		TimeFloat = (float)(DisplayData.signalTime[1][index]) / 10;
		Unix_to_UTC(DisplayData.signalUnix[1][index], &RtcTime);
		sprintf(TimeStr, "%0.1fms %d/%d/%d %d:%d:%d ", TimeFloat, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
		k = 40;
		for( i = 0; i < strlen(TimeStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(TimeStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
#if 0
		for(i = 0; i < (sizeof(buf2) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf2[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
#endif
		return k;
}

static uint16_t GetMechProDataInterTemCode(uint8_t dataDest[2][280])
{
		uint8_t i = 0, j = 0;
		uint16_t k = 0;
		char TempStr[30] = {0};
		char TempTimeStr[20] = {0};
		RtcTime RtcTime;
		//
		//memcpy(&dataDest[0][0], &InterTemCode[0][0], 16);
		//memcpy(&dataDest[1][0], &InterTemCode[1][0], 16);
		//memcpy(&dataDest[0][16], &InterTemCode[2][0], 16);
		//memcpy(&dataDest[1][16], &InterTemCode[3][0], 16);
		memcpy(&dataDest[0][0], &InterTemCode[4][0], 16);
		memcpy(&dataDest[1][0], &InterTemCode[5][0], 16);
		memcpy(&dataDest[0][16], &InterlayerHumiCode[4][0], 16);
		memcpy(&dataDest[1][16], &InterlayerHumiCode[5][0], 16);
		memcpy(&dataDest[0][32], &InterlayerHumiCode[6][0], 16);
		memcpy(&dataDest[1][32], &InterlayerHumiCode[7][0], 16);
		//memcpy(&dataDest[0][48], &InterTemCode[6][0], 16);
		//memcpy(&dataDest[1][48], &InterTemCode[7][0], 16);
		
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if(':' == AsciiIndex[i])
				{
						memcpy(&dataDest[0][48], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][48], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		if(DisplayData.currentInterTempUnix == 0)
		{
				memcpy(&dataDest[0][56], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][56], &NothingCode[1][0], 16);
				return 88;
		}
		
		sprintf(TempStr, "%d", DisplayData.currentCj2301Temp);
		k = 56;
		for(i = 0; i < strlen(TempStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				if(TempStr[i] == AsciiIndex[j])
				{
						memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
						memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
						k += 8;
						break;
				}
		}	
		memcpy(&dataDest[0][k], &TempCode[0][0], 16);
		memcpy(&dataDest[1][k], &TempCode[1][0], 16);
		k += 16;
		memset(TempStr, 0, sizeof(TempStr));
		sprintf(TempStr, " %d%%", DisplayData.currentCj2301Humi);
		for(i = 0; i < strlen(TempStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				if(TempStr[i] == AsciiIndex[j])
				{
						memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
						memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
						k += 8;
						break;
				}
		}	
		Unix_to_UTC(DisplayData.currentInterTempUnix, &RtcTime);
		sprintf(TempTimeStr, " %d/%d/%d %d:%d:%d ", RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
		for(i = 0; i < strlen(TempTimeStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(TempTimeStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
		return k;
}

static uint16_t GetContactTempDataCode(uint8_t dataDest[2][280], uint8_t tempNum)
{
		uint8_t i = 0, j = 0;
		uint16_t k = 0;
		char tempStr[30] = {0};
		char tempTimeStr[20] = {0};
		RtcTime RtcTime;
	
		memcpy(&dataDest[0][0], &ContactTempCode[4][0], 16);
		memcpy(&dataDest[1][0], &ContactTempCode[5][0], 16);
		memcpy(&dataDest[0][16], &ContactTempCode[6][0], 16);
		memcpy(&dataDest[1][16], &ContactTempCode[7][0], 16);
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if((tempNum + '0') == AsciiIndex[i])
				{
						memcpy(&dataDest[0][32], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][32], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if(':' == AsciiIndex[i])
				{
						memcpy(&dataDest[0][40], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][40], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		if(DisplayData.currentTempUnix[tempNum - 1] == 0)
		{
				memcpy(&dataDest[0][48], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][48], &NothingCode[1][0], 16);
				return 64;
		}
		k = 48;
		sprintf(tempStr, "%d", DisplayData.currentTemp[tempNum - 1]);
		for(i = 0; i < strlen(tempStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(tempStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
		memcpy(&dataDest[0][k], &TempCode[0][0], 16);
		memcpy(&dataDest[1][k], &TempCode[1][0], 16);
		k += 16;
		Unix_to_UTC(DisplayData.currentTempUnix[tempNum - 1], &RtcTime);
		sprintf(tempTimeStr, " %d/%d/%d %d:%d:%d ", RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
		for(i = 0; i < strlen(tempTimeStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(tempTimeStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
		return k;
}

static uint16_t GetInterHumiDataCode(uint8_t dataDest[2][280], uint8_t humiVal)
{
		uint8_t i = 0, j = 0;
		//uint8_t buf2[29] = " 23/10/21 23/23/05 ";
		uint16_t k = 0;
		char humiStr[30] = {0};
		RtcTime RtcTime;
	
		memcpy(&dataDest[0][0], &InterlayerHumiCode[4][0], 16);
		memcpy(&dataDest[1][0], &InterlayerHumiCode[5][0], 16);
		memcpy(&dataDest[0][16], &InterlayerHumiCode[6][0], 16);
		memcpy(&dataDest[1][16], &InterlayerHumiCode[7][0], 16);
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if(':' == AsciiIndex[i])
				{
						memcpy(&dataDest[0][32], &AsciiCode[i * 2][0], 8);
						memcpy(&dataDest[1][32], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
		}
		if(DisplayData.currentHumiUnix == 0)
		{
				memcpy(&dataDest[0][40], &NothingCode[0][0], 16);
				memcpy(&dataDest[1][40], &NothingCode[1][0], 16);
				return 56;
		}
		k = 40;
		Unix_to_UTC(DisplayData.currentHumiUnix, &RtcTime);
		sprintf(humiStr, "%d%% %d/%d/%d %d:%d:%d", DisplayData.currentHumi, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
		for(i = 0; i < strlen(humiStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(humiStr[i] == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
		}
		return k;
}

static uint16_t GetDatalogCode(uint8_t dataDest[2][280], uint8_t num, uint8_t page, uint8_t oledPage)
{
		char logStr[30] = {0};
		char unixStr[20] = {0};
		float signalTimeF = 0.0;
		uint32_t unixTime = 0;
		uint8_t logNum = 0;
		uint16_t k = 0;
		uint8_t nullFlag = 0, i = 0, j = 0;
		RtcTime RtcTime;
		logNum = 1 + ((page - 1) * 3) + (oledPage / 2 - 1);
		if(num <= 2)
		{
				if(DisplayData.signalIndex[num - 1][0] < logNum)
				{
						sprintf(logStr, "%d.", logNum);
						nullFlag = 1;
				}
				else
				{
						signalTimeF = (float)(DisplayData.signalTime[num - 1][logNum - 1]) / 10;
						unixTime = DisplayData.signalUnix[num - 1][logNum - 1];
						Unix_to_UTC(unixTime, &RtcTime);
						sprintf(logStr, "%d.%0.1fms %d/%d/%d %d:%d:%d ", logNum, signalTimeF, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
				}
				return k;
		}
		else if(num == 3)
		{
				if(DisplayData.InterTempIndex < logNum)
				{
						sprintf(logStr, "%d.", logNum);
						nullFlag = 1;
				}
				else
				{						
						sprintf(logStr, "%d.%d", logNum, DisplayData.Cj2301Temp[logNum - 1]);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
						return k;
				}
				memcpy(&dataDest[0][k], &TempCode[0][0], 16);
				memcpy(&dataDest[1][k], &TempCode[1][0], 16);
				k += 16;
				memset(logStr, 0, sizeof(logStr));
				sprintf(logStr, " %d%%", DisplayData.Cj2301Humi[logNum - 1]);
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				Unix_to_UTC(DisplayData.InterTempUnix[logNum - 1], &RtcTime);
				sprintf(unixStr, " %d/%d/%d %d:%d:%d ",RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				for(i = 0; i < strlen(unixStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(unixStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else if(num <= 9)
		{
				if(DisplayData.selfTempIndex[num - 4][0] < logNum)
				{
						sprintf(logStr, "%d.", logNum);
						nullFlag = 1;
				}
				else
				{
						sprintf(logStr, "%d.%d", logNum, DisplayData.selfTemp[num - 4][logNum - 1]);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
						return k;
				}
				memcpy(&dataDest[0][k], &TempCode[0][0], 16);
				memcpy(&dataDest[1][k], &TempCode[1][0], 16);
				k += 16;
				Unix_to_UTC(DisplayData.selfTempUnix[num - 4][logNum - 1], &RtcTime);
				sprintf(unixStr, " %d/%d/%d %d:%d:%d ",RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				for(i = 0; i < strlen(unixStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(unixStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else
		{
				if(DisplayData.selfHumiIndex < logNum)
				{
						sprintf(logStr, "%d.", logNum);
						nullFlag = 1;
				}
				else
				{
						Unix_to_UTC(DisplayData.selfHumiUnix[logNum - 1], &RtcTime);
						sprintf(logStr, "%d.%d%% %d/%d/%d %d:%d:%d", logNum, DisplayData.selfHumi[logNum - 1], RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
				}
				return k;
		}
}

static uint16_t GetAbnormalCode(uint8_t dataDest[2][280], uint8_t num, uint8_t page, uint8_t oledPage)
{
		char logStr[30] = {0};
		char unixStr[20] = {0};
		float signalTimeF = 0.0;
		uint32_t unixTime = 0;
		uint8_t logNum = 0;
		uint16_t k = 0;
		uint8_t nullFlag = 0, i = 0, j = 0;
		RtcTime RtcTime;
		logNum = 1 + ((page - 1) * 3) + (oledPage / 2 - 1);
		if(num == 1)
		{
				sprintf(logStr, "%d.", logNum);
				if(DisplayData.abnormnalSignalIndex < logNum)
				{						
						nullFlag = 1;
				}
				else
				{
						nullFlag = 0;
						//signalTimeF = DisplayData.abnormnalSignal[logNum - 1];
						//unixTime = DisplayData.abnormnalSignalUnix[logNum - 1];
						//Unix_to_UTC(unixTime, &RtcTime);
						//sprintf(logStr, ":%0.1fms %d/%d/%d %d:%d:%d ",signalTimeF, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(DisplayData.abnormalSignalType[logNum - 1] == 0)
				{
						memcpy(&dataDest[0][k], &CloseCode[0][0], 16);
						memcpy(&dataDest[1][k], &CloseCode[1][0], 16);
						k += 16;
						memcpy(&dataDest[0][k], &CloseCode[2][0], 16);
						memcpy(&dataDest[1][k], &CloseCode[3][0], 16);
						k += 16;
				}
				else
				{
						memcpy(&dataDest[0][k], &OpenCode[0][0], 16);
						memcpy(&dataDest[1][k], &OpenCode[1][0], 16);
						k += 16;
						memcpy(&dataDest[0][k], &OpenCode[2][0], 16);
						memcpy(&dataDest[1][k], &OpenCode[3][0], 16);
						k += 16;
				}
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(':' == AsciiIndex[j])
						{
								memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
								memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
								k += 8;
								break;
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
						return k;
				}
				memset(logStr, 0, sizeof(logStr));
				signalTimeF = (float)(DisplayData.abnormnalSignal[logNum - 1]) / 10;
				unixTime = DisplayData.abnormnalSignalUnix[logNum - 1];
				Unix_to_UTC(unixTime, &RtcTime);
				sprintf(logStr, "%0.1fms %d/%d/%d %d:%d:%d ",signalTimeF, RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else if(num == 2)
		{
				sprintf(logStr, "%d.", logNum);
				if(DisplayData.abnormalTempIndex < logNum)
				{						
						nullFlag = 1;
				}
				else
				{
						nullFlag = 0;
						//Unix_to_UTC(DisplayData.abnormalTempUnix[logNum - 1], &RtcTime);
						sprintf(logStr, "%d.%d", logNum, DisplayData.abnormalTemp[logNum - 1]);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
						return k;
				}
				memcpy(&dataDest[0][k], &TempCode[0][0], 16);
				memcpy(&dataDest[1][k], &TempCode[1][0], 16);
				k += 16;
				Unix_to_UTC(DisplayData.abnormalTempUnix[logNum - 1], &RtcTime);
				sprintf(unixStr, " %d/%d/%d %d:%d:%d ",RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				for(i = 0; i < strlen(unixStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(unixStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				return k;
		}
		else if(num == 3)
		{
				if(DisplayData.abnormalHumiIndex < logNum)
				{
						sprintf(logStr, "%d.", logNum);
						nullFlag = 1;
				}
				else
				{
						Unix_to_UTC(DisplayData.abnormalHumiUnix[logNum - 1], &RtcTime);
						sprintf(logStr, "%d.%d%% %d/%d/%d %d:%d:%d", logNum, DisplayData.abnormalHumi[logNum - 1], RtcTime.year - 2000, RtcTime.mouth, RtcTime.date, RtcTime.hour, RtcTime.minute, RtcTime.second);
				}
				for(i = 0; i < strlen(logStr); i++)
				{
						for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
						{
								if(logStr[i] == AsciiIndex[j])
								{
										memcpy(&dataDest[0][k], &AsciiCode[j * 2][0], 8);
										memcpy(&dataDest[1][k], &AsciiCode[j * 2 + 1][0], 8);
										k += 8;
										break;
								}
						}
				}
				if(nullFlag)
				{
						memcpy(&dataDest[0][k], &NothingCode[0][0], 16);
						memcpy(&dataDest[1][k], &NothingCode[1][0], 16);
						k += 16;
						
				}
				return k;
		}
		return k;
}

static void GUI_AbnormalTile(uint8_t num, uint8_t page)
{
		uint8_t code[14][8] = {0};
		uint8_t i = 0, j = 0, k = 0;
		char pageStr[8] = {0};
		
		Fill_RAM(0x00);
		sprintf(pageStr, "(%d/10)", page);
		for(i = 0; i < strlen(pageStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(pageStr[i] == AsciiIndex[j])
						{
								memcpy(&code[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								k++;
								break;
						}
				}
		}
		ShowChar(&code[0][0], k, 0, 0, 0);
		k = k * 8;
		switch(num)
		{
			case 1:
				ShowChinese(&MechPeoCode[0][0], 0, k, 4, 0);
				k += 64;
			break;
			case 2:
				ShowChinese(&ContactTempCode[0][0], 0, k, 4, 0);
			break;
			case 3:
				ShowChinese(&InterlayerHumiCode[0][0], 0, k, 4, 0);
			break;
			default:
				
			break;
		}
}

static void AbnormalShowDataMenuFun(void)
{
		static uint8_t step = 0;
		static uint8_t page = 1;
		KeyVal key = KEY_NONE;
		uint8_t Code1[2][280] = {0};
		uint8_t Code2[2][280] = {0};
		uint8_t Code3[2][280] = {0};
		uint16_t CodeLineNum1 = 0;
		uint16_t CodeLineNum2 = 0;
		uint16_t CodeLineNum3 = 0;
		uint16_t maxLine = 0;
		static uint16_t Index1 = 0, Index2 = 0, Index3 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				//page = 1;
				Index1 = 0;
				Index2 = 0;
				Index3 = 0;
				countDelay = 0;
				count = 0;
				delayFlag = FALSE;
				logSelectNum = 1;
				GUI_AbnormalTile(abnormalSelectNum, page);
				CodeLineNum1 = GetAbnormalCode(Code1, abnormalSelectNum, page, 0x02);
				MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
				CodeLineNum2 = GetAbnormalCode(Code2, abnormalSelectNum, page, 0x04);
				MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
				CodeLineNum3 = GetAbnormalCode(Code3, abnormalSelectNum, page, 0x06);
				MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						if(page > 0)
						{
								page--;
								step = 0;
						}
					break;
					case KEY_DOWN_CLICK:						
						if(page < 10)
						{
								page++;
								step = 0;
						}
					break;
					case KEY_BACK_CLICK:
						GUI_Func = AbnormalMenuFun;
						step = 0;
						page = 1;
					break;
					case KEY_ENTER_CLICK:
						//step = 0;
						//page = 1;
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								Index2++;
								Index3++;
								memset(Code1, 0, sizeof(Code1));
								memset(Code2, 0, sizeof(Code2));
								memset(Code3, 0, sizeof(Code3));
								CodeLineNum1 = GetAbnormalCode(Code1, abnormalSelectNum, page, 0x02);
								CodeLineNum2 = GetAbnormalCode(Code2, abnormalSelectNum, page, 0x04);
								CodeLineNum3 = GetAbnormalCode(Code3, abnormalSelectNum, page, 0x06);
								maxLine = CodeLineNum1 >= CodeLineNum2 ? CodeLineNum1 : CodeLineNum2;
								maxLine = maxLine >= CodeLineNum3 ? maxLine : CodeLineNum3;
								CodeLineNum1 = CodeLineNum1 > 128 ? maxLine : CodeLineNum1;
								CodeLineNum2 = CodeLineNum2 > 128 ? maxLine : CodeLineNum2;
								CodeLineNum3 = CodeLineNum3 > 128 ? maxLine : CodeLineNum3;
								if(Index1 >= CodeLineNum1)
								{	
										Index1 = 0;
								}	
								if(Index2 >= CodeLineNum2)
								{
										Index2 = 0;
								}
								if(Index3 >= CodeLineNum3)
								{
										Index3 = 0;
								}
								MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
								MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
								MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
		}
}

static void GUI_AbnormalMenu(uint8_t selectLine)
{
		char buf1[] = "1.";
		char buf2[] = "2.";
		char buf3[] = "3.";
		uint8_t code[12][8] = {0};
		uint8_t i = 0, j = 0, k = 0;
		uint8_t tempPage = 0;
		
		for(i = 0; i < 2; i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&code[k][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[k + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
				k += 2;
		}
		for(i = 0; i < 2; i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf2[i] == AsciiIndex[j])
						{
								memcpy(&code[k][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[k + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
				k += 2;
		}
		for(i = 0; i < 2; i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf3[i] == AsciiIndex[j])
						{
								memcpy(&code[k][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[k + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
				k += 2;
		}
		tempPage = selectLine * 2;
		ShowChinese(&AbnrmalData[2][0], 0x00, 32, 4, 0x00);
		if(tempPage == 0x02)
		{
				DisplayOnePage(0x02, 0x01);
				ShowChar(&code[0][0], 2, 2, 16, 1);
				ShowChinese(&MechPeoCode[0][0], 0x02, 32, 4, 0x01);
				DisplayOnePage(0x04, 0x00);
				ShowChar(&code[4][0], 2, 4, 16, 0);
				ShowChinese(&ContactTempCode[0][0], 0x04, 32, 4, 0x00);
				DisplayOnePage(0x06, 0x00);
				ShowChar(&code[8][0], 2, 6, 16, 0);
				ShowChinese(&InterlayerHumiCode[0][0], 0x06, 32, 4, 0x00);
		}
		else if(tempPage == 0x04)
		{
				DisplayOnePage(0x02, 0x00);
				ShowChar(&code[0][0], 2, 2, 16, 0);
				ShowChinese(&MechPeoCode[0][0], 0x02, 32, 4, 0x00);
				DisplayOnePage(0x04, 0x01);
				ShowChar(&code[4][0], 2, 4, 16, 1);
				ShowChinese(&ContactTempCode[0][0], 0x04, 32, 4, 0x01);
				DisplayOnePage(0x06, 0x00);
				ShowChar(&code[8][0], 2, 6, 16, 0);
				ShowChinese(&InterlayerHumiCode[0][0], 0x06, 32, 4, 0x00);
		}
		else
		{
				DisplayOnePage(0x02, 0x00);
				ShowChar(&code[0][0], 2, 2, 16, 0);
				ShowChinese(&MechPeoCode[0][0], 0x02, 32, 4, 0x00);
				DisplayOnePage(0x04, 0x00);
				ShowChar(&code[4][0], 2, 4, 16, 0);
				ShowChinese(&ContactTempCode[0][0], 0x04, 32, 4, 0x00);
				DisplayOnePage(0x06, 0x01);
				ShowChar(&code[8][0], 2, 6, 16, 1);
				ShowChinese(&InterlayerHumiCode[0][0], 0x06, 32, 4, 0x01);
		}
}

static void AbnormalMenuFun(void)
{
		static uint8_t step = 0;
		static uint8_t SelectLine = 1;
		KeyVal key = KEY_NONE;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Fill_RAM(0x00);
				GUI_AbnormalMenu(SelectLine);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						if(SelectLine > 1)
						{
							SelectLine--;
							
						}
						else
						{
							SelectLine = 3;
						}
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						if(SelectLine < 3)
						{
							SelectLine++;
							
						}
						else
						{
							SelectLine = 1;
						}
						step = 0;
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						SelectLine = 1;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						abnormalSelectNum = SelectLine;
						GUI_Func = AbnormalShowDataMenuFun;
						step = 0;
						SelectLine = 1;
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
			break;
			default:
				
			break;
		}
}

static void GUI_showLog(uint8_t num, uint8_t page)
{
		uint8_t code[14][8] = {0};
		uint8_t i = 0, j = 0, k = 0;
		char pageStr[8] = {0};
		
		Fill_RAM(0x00);
		sprintf(pageStr, "(%d/10)", page);
		for(i = 0; i < strlen(pageStr); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(pageStr[i] == AsciiIndex[j])
						{
								memcpy(&code[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								k++;
								break;
						}
				}
		}
		ShowChar(&code[0][0], k, 0, 0, 0);
		k = k * 8;
		switch(num)
		{
			case 1:
				ShowChinese(&CloseCode[0][0], 0, k, 2, 0);
				k += 32;
			break;
			case 2:
				ShowChinese(&OpenCode[0][0], 0, k, 2, 0);
			break;
			case 3:
				ShowChinese(&TempHumiCode[0][0], 0, k, 3, 0);
			break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				ShowChinese(&ContactTempCode[0][0], 0, k, 4, 0);
				k += 64;
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if((num - 3 + '0') == AsciiIndex[j])
						{
								memcpy(&code[0][0], &AsciiCode[j * 2][0], 8);
								memcpy(&code[1][0], &AsciiCode[j * 2 + 1][0], 8);
						}
				}
				ShowChar(&code[0][0], 1, 0, k, 0);
			break;
			case 10:
				ShowChinese(&InterlayerHumiCode[0][0], 0, k, 4, 0);
			break;
			default:
				
			break;
		}
}

static void DataLogShowDataMenuFun(void)
{
		static uint8_t step = 0;
		static uint8_t page = 1;
		KeyVal key = KEY_NONE;
		uint8_t Code1[2][280] = {0};
		uint8_t Code2[2][280] = {0};
		uint8_t Code3[2][280] = {0};
		uint16_t CodeLineNum1 = 0;
		uint16_t CodeLineNum2 = 0;
		uint16_t CodeLineNum3 = 0;
		uint16_t maxLine = 0;
		static uint16_t Index1 = 0, Index2 = 0, Index3 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				//page = 1;
				Index1 = 0;
				Index2 = 0;
				Index3 = 0;
				countDelay = 0;
				count = 0;
				delayFlag = FALSE;
				GUI_showLog(logSelectNum, page);
				CodeLineNum1 = GetDatalogCode(Code1, logSelectNum, page, 0x02);
				MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
				CodeLineNum2 = GetDatalogCode(Code2, logSelectNum, page, 0x04);
				MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
				CodeLineNum3 = GetDatalogCode(Code3, logSelectNum, page, 0x06);
				MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						if(page > 1)
						{
								page--;
								step = 0;
						}
					break;
					case KEY_DOWN_CLICK:						
						if(page < 10)
						{
								page++;
								step = 0;
						}
					break;
					case KEY_BACK_CLICK:
						GUI_Func = DatalogMenuFun;
						step = 0;
						page = 1;
					break;
					case KEY_ENTER_CLICK:
						//step = 0;
						//page = 1;
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								Index2++;
								Index3++;
								memset(Code1, 0, sizeof(Code1));
								memset(Code2, 0, sizeof(Code2));
								memset(Code3, 0, sizeof(Code3));
								CodeLineNum1 = GetDatalogCode(Code1, logSelectNum, page, 0x02);
								CodeLineNum2 = GetDatalogCode(Code2, logSelectNum, page, 0x04);
								CodeLineNum3 = GetDatalogCode(Code3, logSelectNum, page, 0x06);
								maxLine = CodeLineNum1 >= CodeLineNum2 ? CodeLineNum1 : CodeLineNum2;
								maxLine = maxLine >= CodeLineNum3 ? maxLine : CodeLineNum3;
								CodeLineNum1 = CodeLineNum1 > 128 ? maxLine : CodeLineNum1;
								CodeLineNum2 = CodeLineNum2 > 128 ? maxLine : CodeLineNum2;
								CodeLineNum3 = CodeLineNum3 > 128 ? maxLine : CodeLineNum3;
								if(Index1 >= CodeLineNum1)
								{	
										Index1 = 0;
								}				
								if(Index2 >= CodeLineNum2)
								{
										Index2 = 0;
								}						
								if(Index3 >= CodeLineNum3)
								{
										Index3 = 0;
								}
								MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
								MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
								MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
		}
}


static void GUI_DataLogMenu(void)
{
		Fill_RAM(0x00);
		ShowChinese(&dataLog[2][0], 0, 0, 4, 0x00);
}

static void GUI_DataLogList(uint8_t page, uint8_t listNum, uint8_t selectNum)
{
		uint8_t code[6][8] = {0};
		uint8_t i = 0;
		uint8_t mode = (listNum == selectNum ? 0x01 : 0x00);
		uint8_t k = 0;
		uint8_t index = 0;
		
		if(listNum < 10)
		{
				k = 2;
				for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
				{
						if((listNum + '0') == AsciiIndex[i])
						{
								memcpy(&code[0][0], &AsciiCode[i * 2][0], 8);
								memcpy(&code[1][0], &AsciiCode[i * 2 + 1][0], 8);
								break;
						}
				}
				index += 8;
		}
		else
		{
				k = 4;
				for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
				{
						if((listNum / 10 + '0') == AsciiIndex[i])
						{
								memcpy(&code[0][0], &AsciiCode[i * 2][0], 8);
								memcpy(&code[1][0], &AsciiCode[i * 2 + 1][0], 8);
								break;
						}
				}
				for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
				{
						if((listNum % 10 + '0') == AsciiIndex[i])
						{
								memcpy(&code[2][0], &AsciiCode[i * 2][0], 8);
								memcpy(&code[3][0], &AsciiCode[i * 2 + 1][0], 8);
								break;
						}
				}
				index += 16;
		}
		for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
		{
				if('.' == AsciiIndex[i])
				{
						memcpy(&code[k][0], &AsciiCode[i * 2][0], 8);
						memcpy(&code[k + 1][0], &AsciiCode[i * 2 + 1][0], 8);
						break;
				}
				
		}
		k += 2;
		index += 8;
		DisplayOnePage(page, mode);
		ShowChar(&code[0][0], k / 2, page, 0, mode);
		switch(listNum)
		{
			case 1:
				ShowChinese(&CloseCode[0][0], page, index, 2, mode);
				index += 32;
			break;
			case 2:
				ShowChinese(&OpenCode[0][0], page, index, 2, mode);
				index += 32;
			break;
			case 3:
				ShowChinese(&TempHumiCode[0][0], page, index, 3, mode);
				index += 64;
			break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				ShowChinese(&ContactTempCode[0][0], page, index, 4, mode);
				index += 64;
				for(i = 0; i < (sizeof(AsciiIndex) - 1); i++)
				{
						if((listNum - 3 + '0') == AsciiIndex[i])
						{
								memcpy(&code[0][0], &AsciiCode[i * 2][0], 8);
								memcpy(&code[1][0], &AsciiCode[i * 2 + 1][0], 8);
								break;
						}
				}
				ShowChar(&code[0][0], 1, page, index, mode);
				index += 8;
			break;
			case 10:
				ShowChinese(&InterlayerHumiCode[0][0], page, index, 4, mode);
				index += 64;
			break;
		}
		if(page == 0x02)
		{
				if(listNum > 1)
				{
						ShowChinese(&upDownDirCode[0][0], page, 104, 1, mode);
				}
		}
		else if(page == 0x06)
		{
				if(listNum < 10)
				{
						ShowChinese(&upDownDirCode[2][0], page, 104, 1, mode);
				}
		}
}

void DatalogMenuFun(void)
{
		static uint8_t step = 0;
		static uint8_t startNum = 1, selectNum = 1;
		KeyVal key = KEY_NONE;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				startNum = 1;
				selectNum = 1;
				Fill_RAM(0x00);
				GUI_DataLogMenu();
				GUI_DataLogList(2, startNum, selectNum);
				GUI_DataLogList(4, startNum + 1, selectNum);
				GUI_DataLogList(6, startNum + 2, selectNum);
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						if(selectNum > 1)
						{
								selectNum--;
								if(startNum > selectNum)
								{
										startNum--;
								}
								GUI_DataLogMenu();
								GUI_DataLogList(2, startNum, selectNum);
								GUI_DataLogList(4, startNum + 1, selectNum);
								GUI_DataLogList(6, startNum + 2, selectNum);
						}
					break;
					case KEY_DOWN_CLICK:						
						if(selectNum < 10)
						{
								selectNum++;
								if((selectNum - startNum) >= 3)
								{
										startNum++;
								}
								GUI_DataLogMenu();
								GUI_DataLogList(2, startNum, selectNum);
								GUI_DataLogList(4, startNum + 1, selectNum);
								GUI_DataLogList(6, startNum + 2, selectNum);
						}
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						logSelectNum = selectNum;
						GUI_Func = DataLogShowDataMenuFun;
						step = 0;
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
			break;
			default:
				
			break;
		}
}

static void GUI_InterlayerHumi(void)
{
		uint8_t tempCode[10][8] = {0};
		uint8_t buf1[] = "(4/4)";
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < (sizeof(buf1) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChar(&tempCode[0][0], 5, 0, 0, 0x00);
		ShowChinese(&InterlayerHumiCode[0][0], 0, 40, 4, 0x00);
}

void InterlayerHumidityFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
		uint8_t Code[2][280] = {0};
		uint16_t CodeLineNum = 0;
		static uint16_t Index1 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Index1 = 0;
				delayFlag = FALSE;
				countDelay = 0; 
				count = 0;
				Fill_RAM(0x00);
				GUI_InterlayerHumi();
				CodeLineNum = GetInterHumiDataCode(Code, 40);
				MechProShowScrolling(Code, CodeLineNum, 0x02, Index1);
				
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						GUI_Func = ConactTempSecondFun;
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						GUI_Func = MechProperitesFun;
						step = 0;
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								CodeLineNum = GetInterHumiDataCode(Code, 40);
								if(Index1 >= CodeLineNum)
								{	
										Index1 = 0;
								}
								MechProShowScrolling(Code, CodeLineNum, 0x02, Index1);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
			default:
				
			break;
		}
}

static void GUI_ContactTempSecond(void)
{
		uint8_t tempCode[10][8] = {0};
		uint8_t buf1[] = "(3/4)";
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < (sizeof(buf1) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChar(&tempCode[0][0], 5, 0, 0, 0x00);
		ShowChinese(&ContactTempCode[0][0], 0, 40, 4, 0x00);
}

void ConactTempSecondFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
		uint8_t Code1[2][280] = {0};
		uint8_t Code2[2][280] = {0};
		uint8_t Code3[2][280] = {0};
		uint16_t CodeLineNum1 = 0;
		uint16_t CodeLineNum2 = 0;
		uint16_t CodeLineNum3 = 0;
		uint16_t maxLine = 0;
		static uint16_t Index1 = 0, Index2 = 0, Index3 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Index1 = 0;
				Index2 = 0;
				Index3 = 0;
				delayFlag = FALSE;
				countDelay = 0; 
				count = 0;
				Fill_RAM(0x00);
				GUI_ContactTempSecond();
				CodeLineNum1 = GetContactTempDataCode(Code1, 4);
				MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
				CodeLineNum2 = GetContactTempDataCode(Code2, 5);
				MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
				CodeLineNum3 = GetContactTempDataCode(Code3, 6);
				MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
				
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						GUI_Func = ContactTempFirstFun;
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						GUI_Func = InterlayerHumidityFun;
						step = 0;
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								Index2++;
								Index3++;
								memset(Code1, 0, sizeof(Code1));
								memset(Code2, 0, sizeof(Code2));
								memset(Code3, 0, sizeof(Code3));
								CodeLineNum1 = GetContactTempDataCode(Code1, 4);
								CodeLineNum2 = GetContactTempDataCode(Code2, 5);
								CodeLineNum3 = GetContactTempDataCode(Code3, 6);
								maxLine = CodeLineNum1 >= CodeLineNum2 ? CodeLineNum1 : CodeLineNum2;
								maxLine = maxLine >= CodeLineNum3 ? maxLine : CodeLineNum3;
								CodeLineNum1 = CodeLineNum1 > 128 ? maxLine : CodeLineNum1;
								CodeLineNum2 = CodeLineNum2 > 128 ? maxLine : CodeLineNum2;
								CodeLineNum3 = CodeLineNum3 > 128 ? maxLine : CodeLineNum3;
								if(Index1 >= CodeLineNum1)
								{	
										Index1 = 0;
								}				
								if(Index2 >= CodeLineNum2)
								{
										Index2 = 0;
								}							
								if(Index3 >= CodeLineNum3)
								{
										Index3 = 0;
								}
								MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
								MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
								MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
			default:
				
			break;
		}
}

static void GUI_ContactTemp(void)
{
		uint8_t tempCode[10][8] = {0};
		uint8_t buf1[] = "(2/4)";
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < (sizeof(buf1) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChar(&tempCode[0][0], 5, 0, 0, 0x00);
		ShowChinese(&ContactTempCode[0][0], 0, 40, 4, 0x00);
}


void ContactTempFirstFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
		uint8_t Code1[2][280] = {0};
		uint8_t Code2[2][280] = {0};
		uint8_t Code3[2][280] = {0};
		uint16_t CodeLineNum1 = 0;
		uint16_t CodeLineNum2 = 0;
		uint16_t CodeLineNum3 = 0;
		uint16_t maxLine = 0;
		static uint16_t Index1 = 0, Index2 = 0, Index3 = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0; 
		static uint8_t count = 0;
	
		if(systemFlag.bit.NoActJump)
		{			
				if(step != 0)
				{
						systemFlag.bit.NoActJump = 0;
						step = 0;
				}
		}
		switch(step)
		{
			case 0:
				Index1 = 0;
				Index2 = 0;
				Index3 = 0;
				delayFlag = FALSE;
				countDelay = 0; 
				count = 0;
				Fill_RAM(0x00);
				GUI_ContactTemp();
				CodeLineNum1 = GetContactTempDataCode(Code1, 1);
				MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
				CodeLineNum2 = GetContactTempDataCode(Code2, 2);
				MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
				CodeLineNum3 = GetContactTempDataCode(Code3, 3);
				MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
				
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						GUI_Func = MechProperitesFun;
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						GUI_Func = ConactTempSecondFun;
						step = 0;
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								count = 0;
								Index1++;
								Index2++;			
								Index3++;	
								memset(Code1, 0, sizeof(Code1));
								memset(Code2, 0, sizeof(Code2));
								memset(Code3, 0, sizeof(Code3));
								CodeLineNum1 = GetContactTempDataCode(Code1, 1);
								CodeLineNum2 = GetContactTempDataCode(Code2, 2);
								CodeLineNum3 = GetContactTempDataCode(Code3, 3);
								maxLine = CodeLineNum1 >= CodeLineNum2 ? CodeLineNum1 : CodeLineNum2;
								maxLine = maxLine >= CodeLineNum3 ? maxLine : CodeLineNum3;
								CodeLineNum1 = CodeLineNum1 > 128 ? maxLine : CodeLineNum1;
								CodeLineNum2 = CodeLineNum2 > 128 ? maxLine : CodeLineNum2;
								CodeLineNum3 = CodeLineNum3 > 128 ? maxLine : CodeLineNum3;
								if(Index1 >= CodeLineNum1)
								{	
										Index1 = 0;
								}
											
								if(Index2 >= CodeLineNum2)
								{
										Index2 = 0;
								}													
								if(Index3 >= CodeLineNum3)
								{
										Index3 = 0;
								}
								MechProShowScrolling(Code1, CodeLineNum1, 0x02, Index1);
								MechProShowScrolling(Code2, CodeLineNum2, 0x04, Index2);
								MechProShowScrolling(Code3, CodeLineNum3, 0x06, Index3);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
								
						}
				}
			break;
			default:
				
			break;
		}
}

void GUI_MechProperties(void)
{
		uint8_t buf1[10][8] = {0};
		
		//(1/4)
		memcpy(&buf1[0][0], &AsciiCode[36][0], 8);
		memcpy(&buf1[1][0], &AsciiCode[37][0], 8);
		memcpy(&buf1[2][0], &AsciiCode[2][0], 8);
		memcpy(&buf1[3][0], &AsciiCode[3][0], 8);
		memcpy(&buf1[4][0], &AsciiCode[26][0], 8);
		memcpy(&buf1[5][0], &AsciiCode[27][0], 8);
		memcpy(&buf1[6][0], &AsciiCode[8][0], 8);
		memcpy(&buf1[7][0], &AsciiCode[9][0], 8);
		memcpy(&buf1[8][0], &AsciiCode[38][0], 8);
		memcpy(&buf1[9][0], &AsciiCode[39][0], 8);
				
		ShowChar(&buf1[0][0], 0x05, 0x00, 0x00, 0x00);
		ShowChinese(&MechPeoCode[0][0], 0x00, 0x28, 0x04, 0x00);
		//MechProShowScrolling(index);
}

static uint16_t GetMechProDataTitleCode(uint8_t dataDest[2][280])
{
		uint16_t k = 0;
		
		memcpy(&dataDest[0][0], &AsciiCode[36][0], 8);
		memcpy(&dataDest[1][0], &AsciiCode[37][0], 8);
		memcpy(&dataDest[0][8], &AsciiCode[2][0], 8);
		memcpy(&dataDest[1][8], &AsciiCode[3][0], 8);
		memcpy(&dataDest[0][16], &AsciiCode[26][0], 8);
		memcpy(&dataDest[1][16], &AsciiCode[27][0], 8);
		memcpy(&dataDest[0][24], &AsciiCode[8][0], 8);
		memcpy(&dataDest[1][24], &AsciiCode[9][0], 8);
		memcpy(&dataDest[0][32], &AsciiCode[38][0], 8);
		memcpy(&dataDest[1][32], &AsciiCode[39][0], 8);
		k = 40;
		memcpy(&dataDest[0][40], &MechPeoCode[0][0], 16);
		memcpy(&dataDest[1][40], &MechPeoCode[1][0], 16);
		memcpy(&dataDest[0][56], &MechPeoCode[2][0], 16);
		memcpy(&dataDest[1][56], &MechPeoCode[3][0], 16);
		memcpy(&dataDest[0][72], &MechPeoCode[4][0], 16);
		memcpy(&dataDest[1][72], &MechPeoCode[5][0], 16);
		memcpy(&dataDest[0][88], &MechPeoCode[6][0], 16);
		memcpy(&dataDest[1][88], &MechPeoCode[7][0], 16);
		k = 88 + 16;
		return k;
}

void MechProperitesFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
		//static uint16_t CloseIndex = 0, OpenIndex = 0, InterIndex = 0, titleIndex = 0;
		static uint8_t count = 0;
		uint8_t MechProDataCode0[2][280] = {0};
		uint8_t MechProDataCode1[2][280] = {0};
		uint8_t MechProDataCode2[2][280] = {0};
		uint8_t MechProDataCode3[2][280] = {0};		
		uint16_t MechProDataCloseLine = 0, MechProDataOpenLine = 0, MechProDataInterLine = 0, MechProDataTitleLine = 0;
		static uint16_t scrollNum = 0;
		static bool delayFlag = FALSE;
		static uint16_t countDelay = 0;
		uint16_t maxLine = 0;
	
		switch(step)
		{
			case 0:
#if 0
				CloseIndex = 0;
				OpenIndex = 0;
				InterIndex = 0;
				titleIndex = 0;
#endif
				scrollNum = 0;
				count = 0;
				delayFlag = FALSE;
				countDelay = 0;
				Fill_RAM(0x00);
				//GUI_MechProperties();
				MechProDataTitleLine = GetMechProDataTitleCode(MechProDataCode0);
				MechProShowScrolling(MechProDataCode0, MechProDataTitleLine, 0x00, scrollNum);
				MechProDataCloseLine = GetMechProDataCloseCode(MechProDataCode1);
				MechProShowScrolling(MechProDataCode1, MechProDataCloseLine, 0x02, scrollNum);
				MechProDataOpenLine = GetMechProDataOpenCode(MechProDataCode2);
				MechProShowScrolling(MechProDataCode2, MechProDataOpenLine, 0x04, scrollNum);
				MechProDataInterLine = GetMechProDataInterTemCode(MechProDataCode3);
				MechProShowScrolling(MechProDataCode3, MechProDataInterLine, 0x06, scrollNum);						
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:
						GUI_Func = InterlayerHumidityFun;
						step = 0;
					break;
					case KEY_DOWN_CLICK:
						GUI_Func = ContactTempFirstFun;
						step = 0;
					break;
					case KEY_BACK_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_ENTER_CLICK:
						
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
				if(delayFlag)
				{
						count++;
						if(count >= OLED_SCROING_COUNT)
						{
								memset(MechProDataCode0, 0, sizeof(MechProDataCode1));
								memset(MechProDataCode1, 0, sizeof(MechProDataCode1));
								memset(MechProDataCode2, 0, sizeof(MechProDataCode2));
								memset(MechProDataCode3, 0, sizeof(MechProDataCode3));
								count = 0;
#if 0
								CloseIndex++;
								OpenIndex++;
								InterIndex++;
								titleIndex++;
#endif
								scrollNum++;
								MechProDataTitleLine = GetMechProDataTitleCode(MechProDataCode0);
								MechProDataCloseLine = GetMechProDataCloseCode(MechProDataCode1);
								MechProDataOpenLine = GetMechProDataOpenCode(MechProDataCode2);
								MechProDataInterLine = GetMechProDataInterTemCode(MechProDataCode3);
								maxLine = MechProDataCloseLine >= MechProDataOpenLine ? MechProDataCloseLine : MechProDataOpenLine;
								maxLine = maxLine >= MechProDataInterLine ? maxLine : MechProDataInterLine;
								maxLine = maxLine >= MechProDataTitleLine ? maxLine : MechProDataTitleLine;
								if(maxLine < 128)
								{
										maxLine = 128;
								}
								//MechProDataCloseLine = MechProDataCloseLine > 128 ? maxLine : MechProDataCloseLine;
								//MechProDataOpenLine = MechProDataOpenLine > 128 ? maxLine : MechProDataOpenLine;
								//MechProDataInterLine = MechProDataInterLine > 128 ? maxLine : MechProDataInterLine;
								MechProDataCloseLine = maxLine;
								MechProDataOpenLine = maxLine;
								MechProDataInterLine = maxLine;
								MechProDataTitleLine = maxLine;
								if(scrollNum >= maxLine)
								{
										scrollNum = 0;
								}
#if 0
								if(CloseIndex >= MechProDataCloseLine)
								{
										CloseIndex = 0;
								}														
								if(OpenIndex >= MechProDataOpenLine)
								{
										OpenIndex = 0;
								}																
								if(InterIndex >= MechProDataInterLine)
								{
										InterIndex = 0;
								}
								if(MechProDataCloseLine > 128 && MechProDataOpenLine > 128)
								{
										OpenIndex = CloseIndex;
								}
								if(MechProDataOpenLine > 128 && MechProDataInterLine > 128)
								{
										InterIndex = OpenIndex;
								}
								if(MechProDataCloseLine > 128 && MechProDataInterLine > 128)
								{
										InterIndex = CloseIndex;
								}
#endif
								MechProShowScrolling(MechProDataCode0, MechProDataTitleLine, 0x00, scrollNum);
								MechProShowScrolling(MechProDataCode1, MechProDataCloseLine, 0x02, scrollNum);
								MechProShowScrolling(MechProDataCode2, MechProDataOpenLine, 0x04, scrollNum);
								MechProShowScrolling(MechProDataCode3, MechProDataInterLine, 0x06, scrollNum);
						}
				}
				else
				{
						countDelay++;
						if(countDelay >= 1000)
						{
								countDelay = 0;
								delayFlag = TRUE;
						}
				}
			break;
			default:
				
			break;
		}
}

static void GUI_DeviceInit(void)
{
		uint8_t tempCode[6][8] = {0};
		uint8_t buf1[] = "...";
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < (sizeof(buf1) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChinese(&DeviceInitCode[0][0], 0x01, 0x1C, 0x03, 0x00);
		ShowChar(&tempCode[0][0], 3, 0x01, 0x4C, 0x00);
		ShowChar((uint8_t *)&ProcessBarCode[0][0], 13, 0x06, 13, 0x00);
		
}

void GUI_UpdateProcessBar(uint8_t val)
{
		uint8_t tempBuf[2] = {0xF8, 0x88};
		
		Write8_8_char(tempBuf[0], 0x06, 15, val);
		Write8_8_char(tempBuf[1], 0x06, 15 + val, 100 - val);
}

void GUI_CommuncationFail(void)
{
		ShowChinese(&CommuncationFailCode[0][0], 0x01, 16, 6, 0x00);
		ShowChinese(&ContinusCode[0][0], 6, 96, 2, 0);
}

void CommunicationFailFun(void)
{
		static uint8_t step = 0;
		KeyVal key = KEY_NONE;
	
		switch(step)
		{
			case 0:
				Fill_RAM(0x00);
				GUI_CommuncationFail();
				step++;
			break;
			case 1:
				key = GetButtonVal();
				switch(key)
				{
					case KEY_UP_CLICK:

					break;
					case KEY_DOWN_CLICK:
						
					break;
					case KEY_BACK_CLICK:
						
					break;
					case KEY_ENTER_CLICK:
						GUI_Func = MainMenuFun;
						step = 0;
					break;
					case KEY_NONE:
					case KEY_UP_LONG:
					case KEY_DOWN_LONG:
					case KEY_BACK_LONG:
					case KEY_ENETR_LONG:
						
					break;
					default:
						
					break;
				}
			break;
			default:
				
			break;
		}
}

void DeviceInitFun(void)
{
		static uint8_t step = 0;
		static uint8_t count = 0;
		static uint8_t ProVal = 0;
	
		switch(step)
		{
			case 0:
				Fill_RAM(0x00);
				GUI_DeviceInit();
				step++;
			break;
			case 1:
				count++;
				if(count >= 50)
				{
						count = 0;
						ProVal++;
						GUI_UpdateProcessBar(ProVal);
						if(ProVal == 90)
						{
								if(!systemFlag.bit.Communicate)
								{
									GUI_Func = CommunicationFailFun;
								}
						}
						if(ProVal >= 100)
						{
								GUI_Func = MainMenuFun;
						}						
				}
			break;
			default:
				
			break;
		}
}

static void GUI_DeviceStart(void)
{
		uint8_t tempCode[18][8] = {0};
		uint8_t buf1[] = "MVJC-2000";
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < (sizeof(buf1) - 1); i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(buf1[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChar(&tempCode[0][0], 9, 0x01, 0x1C, 0x00);
		for(i = 0; i < 5; i++)
		{
				for(j = 0; j < (sizeof(AsciiIndex) - 1); j++)
				{
						if(Version[i] == AsciiIndex[j])
						{
								memcpy(&tempCode[i * 2][0], &AsciiCode[j * 2][0], 8);
								memcpy(&tempCode[i * 2 + 1][0], &AsciiCode[j * 2 + 1][0], 8);
								break;
						}
				}
		}
		ShowChar(&tempCode[0][0], 5, 0x05, 0x2C, 0x00);
}

void DeviceStartFun(void)
{
		static uint8_t step = 0;
		static uint16_t count = 0;
	
		switch(step)
		{
			case 0:
				Fill_RAM(0x00);
				GUI_DeviceStart();
				step++;
			break;
			case 1:
				count++;
				if(count >= 2000)
				{
						count = 0;
						step = 0;
						GUI_Func = DeviceInitFun;
				}
			break;
			default:
				
			break;
		}
}
