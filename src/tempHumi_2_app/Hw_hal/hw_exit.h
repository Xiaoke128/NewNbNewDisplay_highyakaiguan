#ifndef HW_EXIT_H
#define HW_EXIT_H

#include "gd32f10x.h"
#include "Init.h"
#include <stdio.h>

#define EXIT_PORT		GPIOB
#define EXIT1_PIN		GPIO_PIN_12
#define EXIT2_PIN		GPIO_PIN_15
#define EXIT3_PIN		GPIO_PIN_14
#define EXIT4_PIN		GPIO_PIN_13

#define EXIT1_SOURCE_PIN		GPIO_PIN_SOURCE_12
#define EXIT2_SOURCE_PIN		GPIO_PIN_SOURCE_15
#define EXIT3_SOURCE_PIN		GPIO_PIN_SOURCE_14
#define EXIT4_SOURCE_PIN		GPIO_PIN_SOURCE_13

#define EXIT1_SOURCE_CHAN		EXTI_12
#define EXIT2_SOURCE_CHAN		EXTI_15
#define EXIT3_SOURCE_CHAN		EXTI_14
#define EXIT4_SOURCE_CHAN		EXTI_13

#define GET_EXIT_CHANNEL1_VAL	gpio_input_bit_get(EXIT_PORT, EXIT1_PIN)
#define GET_EXIT_CHANNEL2_VAL	gpio_input_bit_get(EXIT_PORT, EXIT2_PIN)
#define GET_EXIT_CHANNEL3_VAL	gpio_input_bit_get(EXIT_PORT, EXIT3_PIN)
#define GET_EXIT_CHANNEL4_VAL	gpio_input_bit_get(EXIT_PORT, EXIT4_PIN)

#define MODE_PORT		GPIOA
#define MODE1_PIN		GPIO_PIN_0
#define MODE2_PIN		GPIO_PIN_1

#define GET_MODE1_VAL			gpio_input_bit_get(MODE_PORT, MODE1_PIN)
#define GET_MODE2_VAL			gpio_input_bit_get(MODE_PORT, MODE2_PIN)

#define ACTION_MIN_TIME_MS	50
#define SIGNAL_MIN_TRIGGER_TIME		15

void GetChannel3_Status(void);
void ExitInit(void);
void GetExitPinVal(void);
void SignalCheck(void);
void EXTI10_15_IRQHandler(void);
#endif
