#include "MT29F2G01ABAGDWB.h"
#include "hw_spi.h"
#include "hw_uart.h"

//Read flash id
static uint32_t ReadFlashID(void)
{
		uint32_t temp = 0;
	
		SPI_FLASH_CS_LOW;
	
		/* send "RDID " instruction */
    spi_flash_send_byte(COMMAND_READ_ID);
		/* read a byte from the flash */
    //spi_flash_send_byte(DUMMY_BYTE);

		temp = spi_flash_read_byte();
		/* read a byte from the flash */
    temp = (temp << 8) + spi_flash_read_byte();
	temp = (temp << 8) + spi_flash_read_byte();
	
		SPI_FLASH_CS_HIGH;
	
		return temp;
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
		spi_flash_send_byte(COMMAND_READ_STATUS);
		do
		{
				regTemp = spi_flash_read_byte();
		}while(regTemp & STATUS_MASK_VAL);
		//CommunUartSendBuf(&regTemp, 1);
		SPI_FLASH_CS_HIGH;
}
//earse block
void EarseBlock(uint8_t blockNum)
{
	uint32_t tempBlock = 0;
	if(blockNum == 0) {
		tempBlock = FLASH_BLOCK0_ADDR;
	}
	else {
		tempBlock = FLASH_BLOCK1_ADDR;
	}
	WriteEnable();
	WaitForActionEnd();
		//WriteEnable();
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_EARSE_BLOCK);
		spi_flash_send_byte((uint8_t)(tempBlock >> 16));
		spi_flash_send_byte((uint8_t)(tempBlock >> 8));
		spi_flash_send_byte((uint8_t)(tempBlock));
		SPI_FLASH_CS_HIGH;
		WaitForActionEnd();
}

void ReadDatFromFlash(uint32_t addr, uint8_t *buf, uint16_t len)
{
	uint16_t i = 0;
	WaitForActionEnd();
	SPI_FLASH_CS_LOW;
	spi_flash_send_byte(COMMAND_FAST_READ_DATA);
	spi_flash_send_byte((uint8_t)(addr >> 16));
	spi_flash_send_byte((uint8_t)(addr >> 8));
	spi_flash_send_byte((uint8_t)(addr));
	//spi_flash_send_byte(DUMMY_BYTE);
	for(i = 0; i < len; i++) {
		buf[i] = spi_flash_read_byte();
	}
	SPI_FLASH_CS_HIGH;
}

void WriteDataToFlash(uint32_t addr, uint8_t *buf, uint16_t len)
{
	uint16_t tempLen = len;
	uint16_t i = 0, j = 0;
	uint32_t tempAddr = addr;
	
	while(tempLen > 0) {
		WaitForActionEnd();
		WriteEnable();
		
		SPI_FLASH_CS_LOW;
		spi_flash_send_byte(COMMAND_PROGRAM_DATA);
		spi_flash_send_byte((uint8_t)(tempAddr >> 16));
		spi_flash_send_byte((uint8_t)(tempAddr >> 8));
		spi_flash_send_byte((uint8_t)(tempAddr));
		if(tempLen >= 256) {
			for(i = 0; i < 256; i++) {
				spi_flash_send_byte(buf[j++]);
			}
			tempLen -= 256;
			tempAddr += 256;
		}
		else {
			for(i = 0; i < tempLen; i++) {
				spi_flash_send_byte(buf[j++]);
			}
			tempLen = 0;
		}
		//WaitForActionEnd();
		SPI_FLASH_CS_HIGH;
	}
}

//nand flash init, spi interface init
void NandFlashInit(void)
{
		uint32_t Fid = 0;
	
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
		WriteEnable();
}


void FlashFunTest(void)
{
		static uint8_t writeFlag = 0;
		uint8_t buf[512] = {0x31};

		if(!writeFlag)
		{
			writeFlag = 1;
			EarseBlock(0);
			memset(buf, 0x31, sizeof(buf));
			WriteDataToFlash(FLASH_BLOCK0_ADDR, buf, 512);
			memset(buf, 0, sizeof(buf));
			ReadDatFromFlash(FLASH_BLOCK0_ADDR, buf, 512);
		}
}
