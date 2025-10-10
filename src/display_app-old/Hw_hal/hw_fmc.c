
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

//write data to flash
void WriteStoreInfo(void)
{
	uint32_t size = 0, i = 0;
	uint32_t *addr = (uint32_t *)&StoreInfo;
	uint32_t data = 0;
	
	FlashEarsePage(STORE_INFO_ADDRESS_START);
	fmc_unlock();//unlock
	size = sizeof(StoreInfoStr) >> 2;
	for(i = 0; i < size; i++)
	{
		data = *addr;
		fmc_word_program(STORE_INFO_ADDRESS_START + i * 4, data);
		fmc_flag_clear(FMC_FLAG_BANK0_END);
		fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
		fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
		addr += 1;
	}
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
		StoreInfoStr infoTemp;
		StoreConfStr confTemp;
		uint32_t crc = 0;
		uint8_t i = 0, j = 0, Flag = 0;
	
		memcpy(&infoTemp, (void *)STORE_INFO_ADDRESS_START, sizeof(StoreInfoStr));
		crc = CheckCRC((uint8_t *)&infoTemp, sizeof(StoreInfoStr) - 4);
		if((crc == infoTemp.CrcVal) && (infoTemp.version == 2))
		{
				memcpy(&StoreInfo, &infoTemp, sizeof(StoreInfoStr));
				memcpy(DisplayData.signalIndex, StoreInfo.signalIndex, sizeof(StoreInfo.signalIndex));
				memcpy(DisplayData.signalTime, StoreInfo.signalTime, sizeof(StoreInfo.signalTime));
				memcpy(DisplayData.signalUnix, StoreInfo.signalUnix, sizeof(StoreInfo.signalUnix));
#if 1
				for(i = 0; i < 2; i++)
				{
					for(j = 0; j < 30; j++)
					{
						if(StoreInfo.signalTime[i][j] > 20000)
						{
							Flag = 1;
							break;
						}
					}
					if(Flag)
					{
						memset(&StoreInfo, 0, sizeof(StoreInfoStr));
						StoreInfoStore();
						break;
					}
				}
#endif				
		}
		else
		{
			memset(&StoreInfo, 0, sizeof(StoreInfoStr));
			StoreInfo.version = 2;
			StoreInfoStore();
		}
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

void StoreConfStore(void)
{
		StoreConf.CrcVal = CheckCRC((uint8_t *)&StoreConf, sizeof(StoreConfStr) - 4);
		WriteStoreConf();
}

void StoreInfoStore(void)
{
		StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
		WriteStoreInfo();
}

