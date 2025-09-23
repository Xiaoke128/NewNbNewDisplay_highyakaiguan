#ifndef HW_SPI_H
#define HW_SPI_H
#include <stdio.h>
#include "gd32f10x.h"


#define DUMMY_BYTE       								0xA5

#define SPI_FLASH_CS_LOW            		gpio_bit_reset(GPIOB, GPIO_PIN_6)
#define SPI_FLASH_CS_HIGH           		gpio_bit_set(GPIOB, GPIO_PIN_6)
#define SPI_FLASH_WP_LOW								gpio_bit_reset(GPIOB, GPIO_PIN_7)
#define SPI_FLASH_WP_HIGH								gpio_bit_set(GPIOB, GPIO_PIN_7)


void SpiInit(void);
uint8_t spi_flash_send_byte(uint8_t byte);
uint8_t spi_flash_read_byte(void);

#endif
