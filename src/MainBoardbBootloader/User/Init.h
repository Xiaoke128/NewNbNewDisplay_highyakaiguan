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
#define APP_ADDRESS		0x08003000

#define MODE_PORT		GPIOA
#define MODE1_PIN		GPIO_PIN_0
#define MODE2_PIN		GPIO_PIN_1

#define GET_MODE1_VAL			gpio_input_bit_get(MODE_PORT, MODE1_PIN)
#define GET_MODE2_VAL			gpio_input_bit_get(MODE_PORT, MODE2_PIN)

#define DE_485_PORT		GPIOA
#define DE_485_PIN		GPIO_PIN_7
#define TX_ENABLE		GPIO_BOP(DE_485_PORT) = DE_485_PIN
#define RX_ENABLE		GPIO_BC(DE_485_PORT) = DE_485_PIN

#define NB_POWER_CTR_PORT	GPIOA
#define NB_POWER_CTR_PIN	GPIO_PIN_15
#define NB_POWER_ON			GPIO_BOP(NB_POWER_CTR_PORT) = NB_POWER_CTR_PIN
#define NB_POWER_OFF		GPIO_BC(NB_POWER_CTR_PORT) = NB_POWER_CTR_PIN

#define NB_RESET_PORT		GPIOB
#define NB_RESET_PIN		GPIO_PIN_1
#define NB_RESET_HIGH		GPIO_BOP(NB_RESET_PORT) = NB_RESET_PIN
#define NB_RESET_LOW		GPIO_BC(NB_RESET_PORT) = NB_RESET_PIN

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
