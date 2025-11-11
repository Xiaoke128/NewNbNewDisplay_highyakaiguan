#ifndef INIT_H
#define INIT_H

#include "ds18b20.h"
#include "hw_timer.h"
#include "gd32f10x.h"
#include "systick.h"
#include "hw_uart.h"
#include "hw_exit.h"
#include "hw_i2c.h"
#include "Isl1208.h"
#include "wh_nb73.h"
#include "hw_fmc.h"
#include "hw_wdgt.h"
#include "modbus.h"
#include "MT29F2G01ABAGDWB.h"
#include "protocol.h"
#include "cj2301.h"
#include <stdio.h>
#include <string.h>
#include "commucateSlave.h"
#include "hw_adc.h"

#define APP_VETC_OFFSET		0x4000

#define DE_485_PORT		GPIOA
#define DE_485_PIN		GPIO_PIN_7
#define TX_ENABLE		GPIO_BOP(DE_485_PORT) = DE_485_PIN
#define RX_ENABLE		GPIO_BC(DE_485_PORT) = DE_485_PIN


#define I2C0_PORT		GPIOB
#define I2C0_SCL_PIN	GPIO_PIN_8
#define I2C0_SDA_PIN	GPIO_PIN_9

#define NB_POWER_CTR_PORT	GPIOB
#define NB_POWER_CTR_PIN	GPIO_PIN_0
#define NB_POWER_ON			GPIO_BOP(NB_POWER_CTR_PORT) = NB_POWER_CTR_PIN
#define NB_POWER_OFF		GPIO_BC(NB_POWER_CTR_PORT) = NB_POWER_CTR_PIN

#define NB_RESET_PORT		GPIOB
#define NB_RESET_PIN		GPIO_PIN_1
#define NB_RESET_HIGH		GPIO_BOP(NB_RESET_PORT) = NB_RESET_PIN
#define NB_RESET_LOW		GPIO_BC(NB_RESET_PORT) = NB_RESET_PIN

#define MAX_STORE_INFO_SIZE		48

typedef union {
	uint32_t val;
	struct{
		uint8_t		Time1ms			:1;//1ms 中断
		uint8_t		InterTemp		:1;//上电找到内部温度传感器
		uint8_t		ExterTemp		:1;//上电找到外部温度传感器
		uint8_t 	Run18B20Inter	:1;//内部温度传感器采样
		uint8_t		Run18B20Exter	:1;//外部温度传感器采样
		uint8_t 	Channel1_High	:1;//通道1高电平，可以触发中断
		uint8_t		Channel2_High	:1;//通道1高电平，可以触发中断
		uint8_t 	BackUpSend		:1;//发送备份数据
		uint8_t		SendSuccess		:1;
		uint8_t		ModbusOrNbiot	:1;//0:mobus;1:nbiot
		uint8_t 	Cj2301_Init		:1;//CJ2301是否初始化
		uint8_t		Cj2301_TempHumi		:1;//上电是否找到有线温湿度传感器
		uint8_t 	RunCj2301					:1;//读取有线温湿度传感器
		uint8_t		BypassMode				:1;//上位机可以直接和显示板通信
		uint8_t   	GetChannel3_Status :1;
		uint8_t		AdcReady		:1;
		uint8_t 	AdcNeedSample	:1;
		uint32_t	Reserved		:15;
	}bit;
}SystemFlag;

typedef enum{
		COMMUNICATE_MODBUS,
		COMMUNICATE_NBIOT,
}CommunMode;

typedef union {
	uint8_t val;
	struct{
		uint8_t		KnifeClose		:1;//合闸标志
		uint8_t 	KnifeOpen		:1;//开闸标志
		uint8_t		KnifeFeedBack	:1;//返回标志
		uint8_t		ModeCheck		:1;//0:和T3比较， 1：和T4比较
		uint8_t   	NeedWriteOpenBuf	:1;
		uint8_t   	NeedWriteCloseBuf	:1;
		uint8_t 	KnifeCloseStart		:1;
		uint8_t		KnifeOpenStart		:1;
		//uint8_t		Reserved		:2;
	}bit;
}KnifeBreakInt;

typedef struct {
	uint8_t			KnifeCloseTemp;
	KnifeBreakInt	KnifeFlag;
	uint32_t		Count;//0.1ms
	uint32_t		UseCount;
}SignalInfo;

#pragma pack(4)
typedef struct{
	uint32_t UnixTime;
	uint8_t InterTemp;
	uint8_t ExterTemp;
	uint8_t SignalType;
	uint8_t Ch4_Alarm;
	uint32_t SignalTime;	
}InfoStr;

typedef struct{
	uint16_t Head;
	uint8_t Version;
	uint8_t NumInfo;
	uint8_t NeedSendNum;
	uint8_t reserved;
	uint8_t NumberUseBlock;
	uint8_t CurrentBlock;
	uint32_t BlockStoreBytes[2];
	InfoStr Info[MAX_STORE_INFO_SIZE];
	uint32_t CrcVal;
}StoreInfoStr;
#pragma pack()

typedef struct{
	uint8_t backupNum;
	InfoStr Info[6];
}BackUpStr;

void NetLedToggle(void);
void MCU_Init(void);
void BackUpAdd(void);
void BackUpClearOne(void);

extern char Version[];
extern SystemFlag systemFlag;
extern SignalInfo SigInfo;
extern StoreInfoStr StoreInfo;
extern BackUpStr BackUpInfo;
#endif
