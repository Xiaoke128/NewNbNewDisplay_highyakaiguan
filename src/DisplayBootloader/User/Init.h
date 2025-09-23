#ifndef INIT_H
#define INIT_H

#include "hw_timer.h"
#include "gd32f10x.h"
#include "systick.h"
#include "hw_uart.h"
#include "hw_fmc.h"
#include "hw_wdgt.h"
#include <stdio.h>
#include <string.h>
#include "bootLoader.h"


#define APP_BOOT_FLAG_ADDR	0x20004FFC
#define STAY_BOOT_FLAG_VAL	0x5A5A5A5A
#define APP_ADDRESS		0x08004000


#define GET_LOADER_PIN_VAL		gpio_input_bit_get(GPIOA, GPIO_PIN_12)

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
		uint8_t		InterTemp		:1;//上电找到内部温度传感器
		uint8_t		ExterTemp		:1;//上电找到外部温度传感器
		uint8_t 	Run18B20Inter	:1;//内部温度传感器采样
		uint8_t		Run18B20Exter	:1;//外部温度传感器采样
		uint8_t 	Channel1_High	:1;//通道1高电平，可以触发中断
		uint8_t		Channel2_High	:1;//通道1高电平，可以触发中断
		uint8_t 	BackUpSend		:1;//发送备份数据
		uint8_t		SendSuccess		:1;
		uint32_t	Reserved		:23;
	}bit;
}SystemFlag;


void MCU_Init(void);
void RunApp(uint32_t AppAddr);
extern SystemFlag systemFlag;
#endif
