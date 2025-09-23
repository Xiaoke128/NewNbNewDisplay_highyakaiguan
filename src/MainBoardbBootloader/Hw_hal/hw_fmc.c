
#include "hw_fmc.h"

StoreConfStr StoreConf;

void FlashEarsePage(uint32_t addr)
{
	fmc_unlock();//unlock
	fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
	fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
	fmc_flag_clear(FMC_FLAG_BANK0_END);
	fmc_page_erase(addr);//erase
	fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
	fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
	fmc_flag_clear(FMC_FLAG_BANK0_END);
	fmc_lock();//lock
}

uint32_t CheckCRC(uint8_t *buf, uint16_t len)
{
	uint32_t val = 0;
	uint16_t i =0;
	
	for(i = 0; i < len; i++)
	{
		val += buf[i];
	}
	return val;
}

static void WriteStoreConf(void)
{
	uint32_t size = 0, i = 0;
	uint32_t *addr = (uint32_t *)&StoreConf;
	uint32_t data = 0;
	
	FlashEarsePage(STORE_CONF_ADDRESS_START);
	fmc_unlock();//unlock
	size = sizeof(StoreConfStr) >> 2;
	for(i = 0; i < size; i++)
	{
		data = *addr;
		fmc_word_program(STORE_CONF_ADDRESS_START + i * 4, data);
		fmc_flag_clear(FMC_FLAG_BANK0_END);
		fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
		fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
		addr += 1;
	}
	fmc_lock();//lock
}

void FMC_StoreInfoInit(void)
{
		StoreConfStr confTemp;
		uint32_t crc = 0;
	
		memcpy(&confTemp, (void *)STORE_CONF_ADDRESS_START, sizeof(StoreConfStr));	
		crc = CheckCRC((uint8_t *)&confTemp, sizeof(StoreConfStr) - 4);
		if(crc == confTemp.CrcVal)
		{
				memcpy(&StoreConf, &confTemp, sizeof(StoreConfStr));
		}
		else
		{
				//StoreConfUpdate();
				StoreConf.ReportStartTime = 0;
				memset(&StoreConf.ServerConf, 0, sizeof(ConfStr));
				StoreConf.SlaveAddr = MODBUS_DEFAULT_ADDR;
				StoreConf.BaudRateModbus = 115200;
				StoreConf.CrcVal = CheckCRC((uint8_t *)&StoreConf, sizeof(StoreConfStr) - 4);
				WriteStoreConf();
	}
}


