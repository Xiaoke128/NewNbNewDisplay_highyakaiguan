#ifndef INIT_H
#define INIT_H

#include "hw_timer.h"
#include "gd32f10x.h"
#include "systick.h"
#include "hw_uart.h"
#include "hw_fmc.h"
#include "hw_wdgt.h"
#include "hw_exit.h"
#include "GUI.h"
#include "modbus.h"
#include "wirelessTemp.h"
#include "protocol.h"
#include "commucateSlave.h"
#include <stdio.h>
#include <string.h>

#define APP_VETC_OFFSET		0x4000

#define APP_BOOT_FLAG_ADDR	0x20004FFC
#define STAY_BOOT_FLAG_VAL	0x5A5A5A5A
#define APP_ADDRESS		0x08004000

#define LED_PORT		GPIOB
#define LED1_PIN		GPIO_PIN_3
#define LED2_PIN		GPIO_PIN_4
#define LED3_PIN		GPIO_PIN_1
#define LED4_PIN		GPIO_PIN_0
#define NET_LED_ON		GPIO_BOP(LED_PORT) = LED1_PIN
#define NET_LED_OFF 	GPIO_BC(LED_PORT) = LED1_PIN
#define POWER_LED_ON	GPIO_BOP(LED_PORT) = LED2_PIN
#define POWER_LED_OFF 	GPIO_BC(LED_PORT) = LED2_PIN
#define DATA_GREEN_ON	GPIO_BOP(LED_PORT) = LED3_PIN
#define DATA_GREEN_OFF 	GPIO_BC(LED_PORT) = LED3_PIN
#define DATA_RED_ON		GPIO_BOP(LED_PORT) = LED4_PIN
#define DATA_RED_OFF 	GPIO_BC(LED_PORT) = LED4_PIN

#define DATA_LED_NORMAL			(DATA_GREEN_ON, DATA_RED_OFF)
#define DATA_LED_ABNORMAL_ON	(DATA_GREEN_ON, DATA_RED_ON)
#define DATA_LED_ABNORMAL_OFF	(DATA_GREEN_OFF, DATA_RED_OFF)
#define DATA_LED_ALARM			(DATA_GREEN_OFF, DATA_RED_ON)

typedef  void (*pFunction)(void);

typedef union {
	uint32_t val;
	struct{
		uint8_t		Time1ms			:1;//1ms 中断
		uint8_t		Communicate	:1;//和主板通信状态，0失败，1正常
		uint8_t		NoActJump		:1;
		uint32_t	Reserved		:29;
	}bit;
}SystemFlag;
#pragma pack(4)
typedef struct{
	uint16_t signalTime[2][30];
	uint32_t signalUnix[2][30];
	uint8_t signalIndex[2][1];
	uint16_t version;
	uint32_t CrcVal;
}StoreInfoStr;
#pragma pack()
void LedToggle(void);
void MCU_Init(void);
extern char Version[];
extern SystemFlag systemFlag;
extern pFunction GUI_Func;
extern StoreInfoStr StoreInfo;
#endif
