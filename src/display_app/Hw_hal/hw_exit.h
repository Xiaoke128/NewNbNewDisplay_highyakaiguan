#ifndef HW_EXIT_H
#define HW_EXIT_H

#include "gd32f10x.h"
#include "Init.h"
#include <stdio.h>

#define BUTTON_PORT					GPIOB
#define BUTTON_UP_PIN				GPIO_PIN_12
#define BUTTON_DOWN_PIN			GPIO_PIN_13
#define BUTTON_BACK_PIN			GPIO_PIN_14
#define BUTTON_ENTER_PIN		GPIO_PIN_15

#define BUTTON_UP_SOURCE_PIN			GPIO_PIN_SOURCE_12
#define BUTTON_DOWN_SOURCE_PIN		GPIO_PIN_SOURCE_13
#define BUTTON_BACK_SOURCE_PIN		GPIO_PIN_SOURCE_14
#define BUTTON_ENTER_SOURCE_PIN		GPIO_PIN_SOURCE_15

#define BUTTON_UP_SOURCE_CHAN				EXTI_12
#define BUTTON_DOWN_SOURCE_CHAN			EXTI_13
#define BUTTON_BACK_SOURCE_CHAN			EXTI_14
#define BUTTON_ENTER_SOURCE_CHAN		EXTI_15

#define GET_BUTTON_UP_VAL						gpio_input_bit_get(BUTTON_PORT, BUTTON_UP_PIN)
#define GET_BUTTON_DOWN_VAL					gpio_input_bit_get(BUTTON_PORT, BUTTON_DOWN_PIN)
#define GET_BUTTON_BACK_VAL					gpio_input_bit_get(BUTTON_PORT, BUTTON_BACK_PIN)
#define GET_BUTTON_ENTER_VAL				gpio_input_bit_get(BUTTON_PORT, BUTTON_ENTER_PIN)

#define MODE_PORT		GPIOA
#define MODE1_PIN		GPIO_PIN_0
#define MODE2_PIN		GPIO_PIN_1

#define GET_MODE1_VAL			gpio_input_bit_get(MODE_PORT, MODE1_PIN)
#define GET_MODE2_VAL			gpio_input_bit_get(MODE_PORT, MODE2_PIN)

#define ACTION_MIN_TIME_MS	50
#define SIGNAL_MIN_TRIGGER_TIME		15

#define KEY_NONE_FOR_5_MINUTES		(5 * 60 * 1000)

typedef enum{
	KEY_NONE,
	KEY_UP_CLICK,
	KEY_DOWN_CLICK,
	KEY_BACK_CLICK,
	KEY_ENTER_CLICK,
	KEY_UP_LONG,
	KEY_DOWN_LONG,
	KEY_BACK_LONG,
	KEY_ENETR_LONG,
}KeyVal;

typedef union{
	uint16_t val;
	struct{
		uint8_t keyUpInterrupt			:1;
		uint8_t keyDownInterrupt		:1;
		uint8_t keyBackInterrupt		:1;
		uint8_t keyEnterInterrupt		:1;
		uint16_t reserved						:12;
	}bit;
}ButtonFlag;

void ButtonInit(void);
KeyVal GetButtonVal(void);
void ButtonScan(void);
void EXTI10_15_IRQHandler(void);
#endif
