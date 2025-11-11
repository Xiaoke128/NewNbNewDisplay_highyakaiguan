#include "hw_spi.h"
#include "hw_uart.h"
void SpiInit(void)
{
		spi_parameter_struct spi_init_struct;
	
		spi_i2s_deinit(SPI0);
		/* SPI0_SCK(PB3), SPI0_MISO(PB4) and SPI0_MOSI(PB5) GPIO pin configuration */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_5);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    /* SPI0_CS(PB6) GPIO pin configuration */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		/* WP (PB7) */
		//gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		SPI_FLASH_CS_HIGH;
		//SPI_FLASH_WP_HIGH;
	
		/* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* enable SPI0 */
    spi_enable(SPI0);
}

/*!
    \brief      send a byte through the SPI interface and return the byte received from the SPI bus
    \param[in]  byte: byte to send
    \param[out] none
    \retval     the value of the received byte
*/
uint8_t spi_flash_send_byte(uint8_t byte)
{
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI0, byte);

    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));

    /* return the byte read from the SPI bus */
    return(spi_i2s_data_receive(SPI0));
}


/*!
    \brief      read a byte from the SPI flash
    \param[in]  none
    \param[out] none
    \retval     byte read from the SPI flash
*/
uint8_t spi_flash_read_byte(void)
{
    return(spi_flash_send_byte(DUMMY_BYTE));
}

