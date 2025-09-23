
#include "hw_fmc.h"
#include "modbus.h"
//erase page
static void StorePageErase(uint32_t addr)
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
	
	StorePageErase(STORE_INFO_ADDRESS_START);
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

//write store configure
static void WriteStoreConf(void)
{
	uint32_t size = 0, i = 0;
	uint32_t *addr = (uint32_t *)&StoreConf;
	uint32_t data = 0;
	
	StorePageErase(STORE_CONF_ADDRESS_START);
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
//get crc value
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
//store info init
void FMC_StoreInfoInit(void)
{
	StoreInfoStr infoTemp;
	StoreConfStr confTemp;
	uint32_t crc = 0;
	//Store info
	memcpy(&infoTemp, (void *)STORE_INFO_ADDRESS_START, sizeof(StoreInfoStr));	
	crc = CheckCRC((uint8_t *)&infoTemp, sizeof(StoreInfoStr) - 4);
	if(crc == infoTemp.CrcVal)
	{
		
		if(infoTemp.Version != STORE_INFO_VERSION_3)
		{
			memset(&StoreInfo, 0, sizeof(StoreInfoStr));
			StoreInfo.Head = 0x5A5A;
			StoreInfo.Version = STORE_INFO_VERSION_3;
			StoreInfo.NumInfo = 0;
			StoreInfo.NeedSendNum = 0;
			StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
			WriteStoreInfo();
		}
		else
		{
			memcpy(&StoreInfo, &infoTemp, sizeof(StoreInfoStr));
		}
	}
	else
	{
		memset(&StoreInfo, 0, sizeof(StoreInfoStr));
		StoreInfo.Head = 0x5A5A;
		StoreInfo.Version = STORE_INFO_VERSION_3;
		StoreInfo.NumInfo = 0;
		StoreInfo.NeedSendNum = 0;
		StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
		WriteStoreInfo();
	}
	NbInfo.SendPackNum = StoreInfo.NeedSendNum;
	//store Configure
	memcpy(&confTemp, (void *)STORE_CONF_ADDRESS_START, sizeof(StoreConfStr));	
	crc = CheckCRC((uint8_t *)&confTemp, sizeof(StoreConfStr) - 4);
	if(crc == confTemp.CrcVal)
	{
		memcpy(&StoreConf, &confTemp, sizeof(StoreConfStr));
		NbInfo.ReportStartUtime = StoreConf.ReportStartTime;
		memcpy(&NbInfo.ConfData, &StoreConf.ServerConf, sizeof(ConfStr));
	}
	else
	{
		//StoreConfUpdate();
		StoreConf.ReportStartTime = NbInfo.ReportStartUtime;
		memcpy(&StoreConf.ServerConf, &NbInfo.ConfData, sizeof(ConfStr));
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

void StoreConfUpdate(void)
{
	StoreConf.ReportStartTime = NbInfo.ReportStartUtime;
	memcpy(&StoreConf.ServerConf, &NbInfo.ConfData, sizeof(ConfStr));
	StoreConf.CrcVal = CheckCRC((uint8_t *)&StoreConf, sizeof(StoreConfStr) - 4);
	WriteStoreConf();
}

//Add a info to flash
void StoreInfoAdd(void)
{
	uint8_t i = 0;
	
	Isl1208GetTime();
	if(NbInfo.SendPackNum < MAX_STORE_INFO_SIZE)
	{
		NbInfo.SendPackNum++;
		StoreInfo.NeedSendNum = NbInfo.SendPackNum;
	}
	if(NbInfo.SendDisplayPakNum < MAX_STORE_INFO_SIZE)
	{
			NbInfo.SendDisplayPakNum++;
	}
	if(StoreInfo.NumInfo == MAX_STORE_INFO_SIZE)//store 48 info
	{
		for(i = 1; i < MAX_STORE_INFO_SIZE; i++)
		{
			StoreInfo.Info[i - 1] = StoreInfo.Info[i];//delete the first one
		}
		//add new to last
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].InterTemp = NbInfo.InterTemp;
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].ExterTemp = NbInfo.ExterTemp;
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].SignalType = NbInfo.Stype;
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].SignalTime = NbInfo.Stime;
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].UnixTime = NbInfo.uTime;
		StoreInfo.Info[MAX_STORE_INFO_SIZE - 1].Ch4_Alarm = NbInfo.Chanel4_AlarmFlag;
		StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
		WriteStoreInfo();
	}
	else//less than 48
	{
		StoreInfo.Info[StoreInfo.NumInfo].InterTemp = NbInfo.InterTemp;
		StoreInfo.Info[StoreInfo.NumInfo].ExterTemp = NbInfo.ExterTemp;
		StoreInfo.Info[StoreInfo.NumInfo].SignalType = NbInfo.Stype;
		StoreInfo.Info[StoreInfo.NumInfo].SignalTime = NbInfo.Stime;
		StoreInfo.Info[StoreInfo.NumInfo].UnixTime = NbInfo.uTime;
		StoreInfo.Info[StoreInfo.NumInfo].Ch4_Alarm = NbInfo.Chanel4_AlarmFlag;
		StoreInfo.NumInfo++;
		StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
		WriteStoreInfo();
	}
	NbInfo.Stype = SIGNAL_NO;//clear open or close knife
	NbInfo.Stime = 0;
	NbInfo.Chanel4_AlarmFlag = 0;
}
//Store interval check
void StoreInfoCheck(void)
{
	static uint32_t count = 0;
	StoreConfStr confTemp;
	
	count++;
	if(count >= STORE_INTERVAL_5_MINUTES)//24 hours store once
	{
		count = 0;
		//StoreInfoAdd();
		memcpy(&confTemp, (void *)STORE_CONF_ADDRESS_START, sizeof(StoreConfStr));	
		if(memcmp(&confTemp, &StoreConf, sizeof(StoreConfStr)) != 0)
		{
			NVIC_SystemReset();
		}
	}
}
