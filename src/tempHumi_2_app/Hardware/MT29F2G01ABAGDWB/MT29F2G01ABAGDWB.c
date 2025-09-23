#include "MT29F2G01ABAGDWB.h"
#include "hw_spi.h"
#include "hw_uart.h"

//Read flash id
static uint16_t ReadFlashID(void)
{
		uint16_t temp = 0, temp0 = 0, temp1 = 0;
	
		SPI_FLASH_CS_LOW;
	
		/* send "RDID " instruction */
    spi_flash_send_byte(COMMAND_READ_ID);
		/* read a byte from the flash */
    spi_flash_send_byte(DUMMY_BYTE);

		temp0 = spi_flash_read_byte();
    /* read a byte from the flash */
    temp1 = spi_flash_read_byte();
	
		SPI_FLASH_CS_HIGH;
	
		temp = (temp0 << 8) | temp1;
	
		return temp;
}
//disable write
static void DisableWp(void)
{
		SPI_FLASH_CS_LOW;
	
		spi_flash_send_byte(COMMAND_SET_FEATURE);
		spi_flash_send_byte(REG_BLOCK_LOCK);
		spi_flash_send_byte(DISABLE_WP_REG_VAL);
		SPI_FLASH_CS_HIGH;
}
//write enable
static void WriteEnable(void)
{
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_WRITE_ENABLE);
		SPI_FLASH_CS_HIGH;
}
//wait for earse or write complete
static void WaitForActionEnd(void)
{
		uint8_t regTemp = 0;
	
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_GET_FEATURE);
		do
		{
				regTemp = spi_flash_read_byte();
		}while(regTemp & STATUS_MASK_VAL);
		//CommunUartSendBuf(&regTemp, 1);
		SPI_FLASH_CS_HIGH;
}
//earse block
void EarseBlock(void)
{
		WriteEnable();
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_EARSE_BLOCK);
		spi_flash_send_byte((uint8_t)(USE_BLOCK_ADDR >> 16));
		spi_flash_send_byte((uint8_t)(USE_BLOCK_ADDR >> 8));
		spi_flash_send_byte((uint8_t)(USE_BLOCK_ADDR));
		SPI_FLASH_CS_HIGH;
		WaitForActionEnd();
}


//write data to cache
void WritePageLoad(uint8_t *buf, uint16_t len, uint16_t addr)
{
		uint16_t i = 0;
	
		WriteEnable();
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_PROGRAM_LOAD);
		spi_flash_send_byte(addr >> 8);
		spi_flash_send_byte(addr);
		for(i = 0; i < len; i++)
		{
				spi_flash_send_byte(buf[i]);
		}
		SPI_FLASH_CS_HIGH;
}

//transfor cache to page
void WritePageExe(uint32_t addr)
{
		
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_PROGRAM_EXEU);
		spi_flash_send_byte(addr >> 16);
		spi_flash_send_byte(addr >> 8);
		spi_flash_send_byte(addr);
		SPI_FLASH_CS_HIGH;
		WaitForActionEnd();
}

//read data to cache
void ReadPageToCache(uint32_t addr)
{
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_READ_PAGE_TO_CACHE);
		spi_flash_send_byte(addr >> 16);
		spi_flash_send_byte(addr >> 8);
		spi_flash_send_byte(addr);
		SPI_FLASH_CS_HIGH;
		WaitForActionEnd();
}

//read data from cache
void ReadFromCache(uint8_t *buf, uint16_t len, uint16_t addr)
{
		uint16_t i = 0;
		
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_READ_FROM_CACHE);
		spi_flash_send_byte(addr >> 8);
		spi_flash_send_byte(addr);
		spi_flash_send_byte(DUMMY_BYTE);
		for(i = 0; i < len; i++)
		{
				buf[i] = spi_flash_read_byte();
				//CommunUartSendBuf(&buf[i], 1);
		}
		SPI_FLASH_CS_HIGH;
}
//nand flash init, spi interface init
void NandFlashInit(void)
{
		uint16_t Fid = 0;
	
		SpiInit();
		Fid = ReadFlashID();
		if(Fid == FLASH_ID)
		{
			TX_ENABLE;
			printf("Flash ID is right!");
		}
		else
		{
			TX_ENABLE;
			printf("Flash ID is wrong!");
		}
		DisableWp();
		//WriteEnable();
		EarseBlock();
}


void FlashFunTest(void)
{
		static uint8_t writeFlag = 0;
		uint8_t buf[512] = {0x31};
		uint16_t len = 512;
		uint16_t addr = 0;
		uint8_t i = 0;

		if(!writeFlag)
		{
				writeFlag = 1;
				memset(buf, 0x31, sizeof(buf));
				for(i = 0; i < 4; i++)
				{
						WritePageLoad(buf, len, addr);
						WritePageExe(USE_BLOCK_ADDR);
						addr += len;
						delay_1ms(5);
				}
				memset(buf, 0x30, sizeof(buf));
				
				ReadPageToCache(USE_BLOCK_ADDR);
				delay_1ms(5);
				addr = 0;
				len = 128;
				for(i = 0; i < 16; i++)
				{	
						ReadFromCache(buf, len, addr);
						addr += len;
						//CommunUartSendBuf(buf, len);
						//delay_1ms(5);
				}
		}
}
