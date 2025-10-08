#ifndef __OLED_NEW_H__
#define __OLED_NEW_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gd32f10x.h"
#include "systick.h"


//#define OLED_RESET_LOW            		gpio_bit_reset(GPIOA, GPIO_PIN_9)
//#define OLED_RESET_HIGH           		gpio_bit_set(GPIOA, GPIO_PIN_9)

#define OLED_CMD_MODE            		gpio_bit_reset(GPIOA, GPIO_PIN_10)
#define OLED_DATA_MODE           		gpio_bit_set(GPIOA, GPIO_PIN_10)

void OLED_Init_E(void);

#endif
