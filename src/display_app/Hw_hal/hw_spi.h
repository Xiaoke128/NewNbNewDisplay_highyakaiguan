#ifndef HW_SPI_H
#define HW_SPI_H
#include <stdio.h>
#include "gd32f10x.h"


#define DUMMY_BYTE       								0xA5

#define SPI_CS_LOW            		gpio_bit_reset(GPIOA, GPIO_PIN_8)
#define SPI_CS_HIGH           		gpio_bit_set(GPIOA, GPIO_PIN_8)


#define OLED_CLK_HIGH			GPIO_BOP(GPIOA) = GPIO_PIN_5
#define OLED_CLK_LOW			GPIO_BC(GPIOA) = GPIO_PIN_5

#define OLED_DATA_HIGH		GPIO_BOP(GPIOA) = GPIO_PIN_7
#define OLED_DATA_LOW			GPIO_BC(GPIOA) = GPIO_PIN_7

void SpiInit(void);
void spi_send_byte(uint8_t byte);

#endif
