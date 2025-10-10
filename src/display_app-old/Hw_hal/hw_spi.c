#include "hw_spi.h"
#include "systick.h"
//#define SPI_HARDWARE
void SpiInit(void)
{
#ifdef SPI_HARDWARE
		spi_parameter_struct spi_init_struct;
	
		spi_i2s_deinit(SPI0);
		/* SPI0_SCK(PA5), SPI0_MISO(PB4) and SPI0_MOSI(PA7) GPIO pin configuration */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* SPI0_CS(PB6) GPIO pin configuration */
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
		//gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		SPI_CS_HIGH;
		//SPI_FLASH_WP_HIGH;
	
		/* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* enable SPI0 */
    spi_enable(SPI0);
#else
		//cs
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
		SPI_CS_HIGH;
		
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
		
		gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		
		OLED_CLK_LOW;
		OLED_DATA_LOW;
#endif
}

/*!
    \brief      send a byte through the SPI interface and return the byte received from the SPI bus
    \param[in]  byte: byte to send
    \param[out] none
    \retval     the value of the received byte
*/
void spi_send_byte(uint8_t Data)
{
#ifdef SPI_HARDWARE
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI0, Data);

		//while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
    /* wait to receive a byte */
    //while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));

		//spi_i2s_data_receive(SPI0);
    /* return the byte read from the SPI bus */
    //return(spi_i2s_data_receive(SPI0));
		//return;
#else
	uint8_t i = 0;
	for (i=0; i<8; i++)
	{
		OLED_CLK_LOW;
		if((Data&0x80) >> 7)
		{
				OLED_DATA_HIGH;
		}
		else
		{
				OLED_DATA_LOW;
		}
		//SDIN=(Data&0x80)>>7;
		Data = Data << 1;
//		uDelay(1);
		OLED_CLK_HIGH;
//		uDelay(1);
	}
#endif
}


