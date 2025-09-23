#ifndef DS18B20_H
#define DS18B20_H
#include "gd32f10x.h"
#include "systick.h"
#include "Init.h"

#define BS18B20_PORT			GPIOA
#define BS18B20_PIN				GPIO_PIN_6

#define EXT_BS18B20_PORT		GPIOA
#define EXT_BS18B20_PIN			GPIO_PIN_8

//IO方向设置
#define DS18B20_IO_IN() {GPIO_CTL0(GPIOA)&=~GPIO_MODE_MASK(6);GPIO_CTL0(GPIOA)|=GPIO_MODE_SET(6,0x8);}//PA6输入模式
#define DS18B20_IO_OUT() {GPIO_CTL0(GPIOA)&=~GPIO_MODE_MASK(6);GPIO_CTL0(GPIOA)|=GPIO_MODE_SET(6,0x3);}//PA6输出模式


#define EXT_DS18B20_IO_IN() {GPIO_CTL1(GPIOA)&=~GPIO_MODE_MASK(0);GPIO_CTL1(GPIOA)|=GPIO_MODE_SET(0,0x8);}//PA8输入模式
#define EXT_DS18B20_IO_OUT() {GPIO_CTL1(GPIOA)&=~GPIO_MODE_MASK(0);GPIO_CTL1(GPIOA)|=GPIO_MODE_SET(0,0x3);}//PA8输出模式

////IO操作函数											    
#define	DS18B20_DQ_IN gpio_input_bit_get(BS18B20_PORT,BS18B20_PIN)
#define	EXT_DS18B20_DQ_IN gpio_input_bit_get(EXT_BS18B20_PORT,EXT_BS18B20_PIN)

typedef enum{
	TEMP_INTER,
	TEMP_EXTER,
}TempType;

void DS18B20_Init(void);//初始化DS18B20 
void DS18B20GetIntTemp(void);
void DS18B20GetExtTemp(void);

#endif
