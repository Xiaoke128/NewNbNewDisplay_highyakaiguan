#include "modbus.h"
#include "hw_uart.h"

ModbusStr modbus;
static ModbusTimer modTimer;
ModbusTempHumiStr TempHumiData;

static uint16_t crcTb[];

static uint16_t GetSlaveId(void);
static uint16_t GetBaudRateH(void);
static uint16_t GetBaudRateL(void);
static uint16_t GetRTC_TimeH(void);
static uint16_t GetRTC_TimeL(void);
static uint16_t GetTemSimpleInterval(void);
static uint16_t GetCloseAlarm(void);
static uint16_t GetOpenAlarm(void);
static uint16_t GetCloseAbnormal(void);
static uint16_t GetOpenAbnormal(void);
static uint16_t GetTemAbnormal(void);
static uint16_t GetHumiAbnormal(void);
static uint16_t GetTemDiffAlarm(void);
static uint16_t GetHwVer(void);
static uint16_t GetInterTem(void);	
static uint16_t GetExterTem(void);
static uint16_t GetHumi(void);
static uint16_t GetChannel4Alarm(void);
static uint16_t GetOpenCloseFlag(void);
static uint16_t GetOpenCloseTime(void);
static uint16_t GetUniqueID_HLLL(void);
static uint16_t GetUniqueID_LHLL(void);
static uint16_t GetUniqueID_LLHL(void);
static uint16_t GetUniqueID_LLLH(void);
static uint16_t GetOpenCloseUnixH(void);
static uint16_t GetOpenCloseUnixL(void);
static uint16_t GetCloseFlag(void);
static uint16_t GetCloseTime(void);
static uint16_t GetCloseUnixH(void);
static uint16_t GetCloseUnixL(void);
static uint16_t GetWaterTempDiff(void);
static uint16_t GetWaterTemp(void);
static uint16_t GetWl_Max(void);
static uint16_t GetWl_Min(void);
static uint16_t GetWl_Diff(void);
static uint16_t GetDeviceStatus(void);
static uint16_t GetCh3_Set(void);

//write function
static void SetSlaveID(uint16_t data);
static void SetBaudRateH(uint16_t data);
static void SetBaudRateL(uint16_t data);
static void SetRtcH(uint16_t data);
static void SetRtcL(uint16_t data);
static void SetTemSimpleInterval(uint16_t data);
static void SetCloseAlarm(uint16_t data);
static void SetOpenAlarm(uint16_t data);
static void SetCloseAbnormal(uint16_t data);
static void SetOpenAbnormal(uint16_t data);
static void SetTemAbnormal(uint16_t data);
static void SetHumiAbnormal(uint16_t data);
static void SetTemDiffAlarm(uint16_t data);
static void SetSelfTemp1(int16_t data);
static void SetSelfTemp2(int16_t data);
static void SetSelfTemp3(int16_t data);
static void SetSelfTemp4(int16_t data);
static void SetSelfTemp5(int16_t data);
static void SetSelfTemp6(int16_t data);
static void SetSelfHumi(int16_t data);
static void SetCh3_Set(uint16_t data);
//static void  SetCloseTime(uint16_t data);
//static void SetOpenTime(uint16_t data);



FunTable table[] = {
	{REG_SLAVE_ADDR,										GetSlaveId},
	{REG_BAUD_RATE_H,										GetBaudRateH},
	{REG_BAUD_RATE_L,										GetBaudRateL},
	{REG_RTC_TIME_H,										GetRTC_TimeH},
	{REG_RTC_TIME_L,										GetRTC_TimeL},
	{REG_TEM_INTERVAL,										GetTemSimpleInterval},
	{REG_CLOSE_ALARM,										GetCloseAlarm},
	{REG_OPEN_ALARM,										GetOpenAlarm},
	{REG_CLOSE_ABNORMAL,									GetCloseAbnormal},
	{REG_OPEN_ABNORMAL,										GetOpenAbnormal},
	{REG_TEM_ABNORMAL,										GetTemAbnormal},
	{REG_HUMI_ABNORMAL,										GetHumiAbnormal},
	{REG_TEM_DIFF_ALARM,									GetTemDiffAlarm},
	{REG_HW_VERSION,										GetHwVer},
	{REG_INTERL_TEM,										GetInterTem},
	{REG_EXTERL_TEM,										GetExterTem},
	{REG_HUMIDUTY,											GetHumi},
	{REG_CHANNEL_4_ALARM,									GetChannel4Alarm},
	{REG_OPEN_FLAG,											GetOpenCloseFlag},
	{REG_OPEN_TIME,											GetOpenCloseTime},
	{REG_OPEN_UNIX_H,										GetOpenCloseUnixH},
	{REG_OPEN_UNIX_L,										GetOpenCloseUnixL},
	{REG_UNIQUE_IDHLLL,										GetUniqueID_HLLL},
	{REG_UNIQUE_IDLHLL,										GetUniqueID_LHLL},
	{REG_UNIQUE_IDLLHL,										GetUniqueID_LLHL},
	{REG_UNIQUE_IDLLLH,										GetUniqueID_LLLH},
	{REG_CLOSE_FLAG,										GetCloseFlag},
	{REG_CLOSE_TIME,										GetCloseTime},
	{REG_CLOSE_UNIX_H,										GetCloseUnixH},
	{REG_CLOSE_UNIX_L,										GetCloseUnixL},
	{REG_WATER_TEMP_DIFF,									GetWaterTempDiff},
	{REG_WATER_TEMP,										GetWaterTemp},
	{REG_WL_TEMP_MAX,										GetWl_Max},
	{REG_WL_TEMP_MIN,										GetWl_Min},
	{REG_WL_TEMP_MAX_MIN_DIFF,								GetWl_Diff},
	{REG_DEVICE_STATUS,										GetDeviceStatus},
	{REG_CH3_SET,											GetCh3_Set},
};

WriteFunTable Wtable[] = {
	{REG_SLAVE_ADDR,										SetSlaveID},
	{REG_BAUD_RATE_H,										SetBaudRateH},
	{REG_BAUD_RATE_L,										SetBaudRateL},
	{REG_RTC_TIME_H,										SetRtcH},
	{REG_RTC_TIME_L,										SetRtcL},
	{REG_TEM_INTERVAL,										SetTemSimpleInterval},
	{REG_CLOSE_ALARM,										SetCloseAlarm},
	{REG_OPEN_ALARM,										SetOpenAlarm},
	{REG_CLOSE_ABNORMAL,									SetCloseAbnormal},
	{REG_OPEN_ABNORMAL,										SetOpenAbnormal},
	{REG_TEM_ABNORMAL,										SetTemAbnormal},
	{REG_HUMI_ABNORMAL,										SetHumiAbnormal},
	{REG_TEM_DIFF_ALARM,									SetTemDiffAlarm},
	{REG_CH3_SET,											SetCh3_Set},
};

SelfWriteFunTable WtempHumiTable[] = {
	{REG_SELF_TEMP1,				SetSelfTemp1},
	{REG_SELF_TEMP2,				SetSelfTemp2},
	{REG_SELF_TEMP3, 				SetSelfTemp3},
	{REG_SELF_TEMP4,				SetSelfTemp4},
	{REG_SELF_TEMP5,				SetSelfTemp5},
	{REG_SELF_TEMP6,				SetSelfTemp6},
	{REG_SELF_HUMI,					SetSelfHumi},
	//{REG_CLOSE_TIME,				SetCloseTime},
	//{REG_OPEN_TIME,					SetOpenTime},
};

static void SetSelfTemp1(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[0] = data;
				TempHumiData.selfTempUnix[0] = NbInfo.uTime;
		}
}
static void SetSelfTemp2(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[1] = data;
				TempHumiData.selfTempUnix[1] = NbInfo.uTime;
		}
}
static void SetSelfTemp3(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[2] = data;
				TempHumiData.selfTempUnix[2] = NbInfo.uTime;
		}
}
static void SetSelfTemp4(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[3] = data;
				TempHumiData.selfTempUnix[3] = NbInfo.uTime;
		}
}
static void SetSelfTemp5(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[4] = data;
				TempHumiData.selfTempUnix[4] = NbInfo.uTime;
		}
}
static void SetSelfTemp6(int16_t data)
{
		if(((data / 10) >= (-50)) && ((data / 10) <= 125)) {
				TempHumiData.selfTemp[5] = data;
				TempHumiData.selfTempUnix[5] = NbInfo.uTime;
		}
}
static void SetSelfHumi(int16_t data)
{
		TempHumiData.selfHumi = data;
		TempHumiData.selfHumiUnix = NbInfo.uTime;
}
#if 0
static void  SetCloseTime(uint16_t data)
{
		NbInfo.Stype = SIGNAL_CLOSE;
		NbInfo.Stime = data;
		StoreInfoAdd();
}

static void SetOpenTime(uint16_t data)
{
		NbInfo.Stype = SIGNAL_OPEN;
		NbInfo.Stime = data;
		StoreInfoAdd();
}
#endif
static uint16_t GetSlaveId(void)
{
		return StoreConf.SlaveAddr;
}

static uint16_t GetBaudRateH(void)
{
		return StoreConf.BaudRateModbus >> 16;
}

static uint16_t GetBaudRateL(void)
{
		return StoreConf.BaudRateModbus;
}
static uint16_t GetRTC_TimeH(void)
{
		//Isl1208GetTime();
		return NbInfo.uTime >> 16;
}

static uint16_t GetRTC_TimeL(void)
{
		//Isl1208GetTime();
		return NbInfo.uTime;
}

static uint16_t GetTemSimpleInterval(void)
{
		return NbInfo.ConfData.TemCollectInterval;
}

static uint16_t GetCloseAlarm(void)
{
		return (uint16_t)NbInfo.ConfData.CloseAlramTime;
}

static uint16_t GetOpenAlarm(void)
{
		return (uint16_t)NbInfo.ConfData.OpenAlarmTime;
}

static uint16_t GetCloseAbnormal(void)
{
		return (uint16_t)NbInfo.ConfData.AbnormalCloseTime;
}

static uint16_t GetOpenAbnormal(void)
{
		return (uint16_t)NbInfo.ConfData.AbnormalOpenTime;
}

static uint16_t GetTemAbnormal(void)
{
		return NbInfo.ConfData.AbnormalTem;
}

static uint16_t GetHumiAbnormal(void)
{
		return NbInfo.ConfData.AbnormalHumi;
}

static uint16_t GetTemDiffAlarm(void)
{
		return NbInfo.ConfData.TempDiffAlarmVal;
}

static uint16_t GetHwVer(void)
{
		return 0x0002;
}

static uint16_t GetInterTem(void)
{
		return NbInfo.InterTemp;
}

static uint16_t GetExterTem(void)
{
		return NbInfo.Cj2301Temp / 10 + 100;
}

static uint16_t GetHumi(void)
{
		return NbInfo.Cj2301Humi / 10;
}

static uint16_t GetChannel4Alarm(void)
{
		uint16_t ret = 0;
	
		if(NbInfo.Chanel4_AlarmFlag)
		{
				NbInfo.Chanel4_AlarmFlag = 0;
				ret	= 0x0001;
				//return 0x0001;
		}
		ret += (uint16_t)(NbInfo.Channel3_Status << 8);
		return ret;
}

static uint8_t readFlag = 0;

static uint16_t GetOpenCloseFlag(void)
{
		if((NbInfo.SendPackNum > 0) && (StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalType == 0x01))
		{
				readFlag = 1;
				return 0x0001;
		}
		return 0x0000;
}

static uint16_t GetCloseFlag(void)
{
		if((NbInfo.SendPackNum > 0) && (StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalType == 0x02))
		{
				readFlag = 2;
				return 0x0001;
		}
		return 0x0000;
}

static uint16_t GetOpenCloseTime(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 1)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime;
				//NbInfo.SendPackNum--;
				//StoreInfo.NeedSendNum = NbInfo.SendPackNum;
				//StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
				//WriteStoreInfo();
				return ret;
		}
		return 0x0000;
}

static uint16_t GetCloseTime(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 2)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime;
				//NbInfo.SendPackNum--;
				//StoreInfo.NeedSendNum = NbInfo.SendPackNum;
				//StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
				//WriteStoreInfo();
				return ret;
		}
		return 0x0000;
}

static uint16_t GetOpenCloseUnixH(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 1)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime >> 16;
				return ret;
		}
		return 0x0000;
}

static uint16_t GetCloseUnixH(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 2)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime >> 16;
				return ret;
		}
		return 0x0000;
}

static uint16_t GetOpenCloseUnixL(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 1)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime;
				NbInfo.SendPackNum--;
				StoreInfo.NeedSendNum = NbInfo.SendPackNum;
				StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
				WriteStoreInfo();
				readFlag = 0;
				return ret;
		}
		return 0x0000;
}

static uint16_t GetCloseUnixL(void)
{
		uint16_t ret = 0;
		if(NbInfo.SendPackNum > 0 && readFlag == 2)
		{
				ret = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime;
				NbInfo.SendPackNum--;
				StoreInfo.NeedSendNum = NbInfo.SendPackNum;
				StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
				WriteStoreInfo();
				readFlag = 0;
				return ret;
		}
		return 0x0000;
}

static uint16_t GetUniqueID_HLLL(void)
{
		uint16_t ret = (uint16_t)(NbInfo.IMEI_BCD[0] << 8) + (uint16_t)NbInfo.IMEI_BCD[1];
	
		return ret;
}

static uint16_t GetUniqueID_LHLL(void)
{
		uint16_t ret = (uint16_t)(NbInfo.IMEI_BCD[2] << 8) + (uint16_t)NbInfo.IMEI_BCD[3];
	
		return ret;
}

static uint16_t GetUniqueID_LLHL(void)
{
		uint16_t ret = (uint16_t)(NbInfo.IMEI_BCD[4] << 8) + (uint16_t)NbInfo.IMEI_BCD[5];
	
		return ret;
}

static uint16_t GetUniqueID_LLLH(void)
{
		uint16_t ret = (uint16_t)(NbInfo.IMEI_BCD[6] << 8) + (uint16_t)NbInfo.IMEI_BCD[7];
	
		return ret;
}

static uint16_t GetWaterTempDiff(void)
{
		float temp = ((float)(NbInfo.Cj2301Temp / 10) -  NbInfo.waterTemp) * 10;
		
		uint16_t ret = (uint16_t)temp;
		return ret;
}

static uint16_t GetWaterTemp(void)
{
		uint16_t ret = 0;
		float temp = (NbInfo.waterTemp + 100.0) * 10.0;
		
		ret = (uint16_t)temp;
		return ret;
}

static int16_t CheckTemMax(void)
{
		int16_t ret = 0;
		uint8_t i = 0;
		uint8_t HaveNormalFlag = 0;
	
	
		for(i = 0; i < 6; i++)
		{
				if((TempHumiData.selfTemp[i] / 10 > -50) && (TempHumiData.selfTemp[i] / 10 <= 125))
				{
						HaveNormalFlag = 1;
						ret = TempHumiData.selfTemp[i];
						break;
				}
		}
		if(HaveNormalFlag)
		{
				for(i = 0; i < 6; i++)
				{	
						if((TempHumiData.selfTemp[i] / 10 > -50) && (TempHumiData.selfTemp[i] / 10 <= 125))
						{
								if(TempHumiData.selfTemp[i] > ret)
								{
										ret = TempHumiData.selfTemp[i];
								}
						}
				}
		}
		return ret;
}

static int16_t CheckTemMin(void)
{
		int16_t ret = 0;
		uint8_t i = 0;
		uint8_t HaveNormalFlag = 0;
	
	
		for(i = 0; i < 6; i++)
		{
				if((TempHumiData.selfTemp[i] / 10 > -50) && (TempHumiData.selfTemp[i] / 10 <= 125))
				{
						HaveNormalFlag = 1;
						ret = TempHumiData.selfTemp[i];
						break;
				}
		}
		if(HaveNormalFlag)
		{
				for(i = 0; i < 6; i++)
				{	
						if((TempHumiData.selfTemp[i] / 10 > -50) && (TempHumiData.selfTemp[i] / 10 <= 125))
						{
								if(TempHumiData.selfTemp[i] < ret)
								{
										ret = TempHumiData.selfTemp[i];
								}
						}
				}
		}
		return ret;
}

static uint16_t GetWl_Max(void)
{
		int16_t maxTemp = 0;
		
		maxTemp = CheckTemMax();
		return (100 + maxTemp / 10) * 10;
		//return (100 + TempHumiData.selfTemp[0] / 10 - TempHumiData.selfTemp[3] / 10) * 10;
}

static uint16_t GetWl_Min(void)
{
		int16_t MinTemp = 0;

		MinTemp = CheckTemMin();
		return (100 + MinTemp / 10) * 10;
		//return (100 + TempHumiData.selfTemp[1] / 10 - TempHumiData.selfTemp[4] / 10) * 10;
}

static uint16_t GetWl_Diff(void)
{
		int16_t maxTemp = 0;
		int16_t MinTemp = 0;
	
		maxTemp = CheckTemMax();
		MinTemp = CheckTemMin();
		return (100 + (maxTemp - MinTemp) / 10) * 10;
}

static uint16_t GetDeviceStatus(void)
{
	return (NbInfo.DataLedStatus ? 1 : 0);
}

static uint16_t GetCh3_Set(void)
{
	return StoreConf.Ch3_Set;
}


static void SetSlaveID(uint16_t data)
{
		StoreConf.SlaveAddr = data;
		StoreConfStore();
		SendMainBoardInfoEnable();
}

static void SetBaudRateH(uint16_t data)
{
		StoreConf.BaudRateModbus = (uint32_t)(data << 16);
		SendMainBoardInfoEnable();
}

static void SetBaudRateL(uint16_t data)
{
		StoreConf.BaudRateModbus += data;
		StoreConfStore();
		UartHostReinit();
}

static uint16_t RtcH_data = 0;
static void SetRtcH(uint16_t data)
{
		RtcH_data = data;
}

static void SetRtcL(uint16_t data)
{
		NbInfo.uTime = (uint32_t)(RtcH_data << 16) + data;
		Unix_to_UTC(NbInfo.uTime, &TimeStr);
		RtcSetTime(TimeStr);
}


static void SetTemSimpleInterval(uint16_t data)
{
		NbInfo.ConfData.TemCollectInterval = data;
		StoreConfUpdate();
}

static void SetCloseAlarm(uint16_t data)
{
		NbInfo.ConfData.CloseAlramTime = data;
		StoreConfUpdate();
}


static void SetOpenAlarm(uint16_t data)
{
		NbInfo.ConfData.OpenAlarmTime = data;
		StoreConfUpdate();
}

static void SetCloseAbnormal(uint16_t data)
{
		NbInfo.ConfData.AbnormalCloseTime = data;
		StoreConfUpdate();
}

static void SetOpenAbnormal(uint16_t data)
{
		NbInfo.ConfData.AbnormalOpenTime = data;
		StoreConfUpdate();
}

static void SetTemAbnormal(uint16_t data)
{
		NbInfo.ConfData.AbnormalTem = data;
		StoreConfUpdate();
}

static void SetHumiAbnormal(uint16_t data)
{
		NbInfo.ConfData.AbnormalHumi = data;
		//do nothing, no humiduty
}

static void SetTemDiffAlarm(uint16_t data)
{
		NbInfo.ConfData.TempDiffAlarmVal = data;
		StoreConfUpdate();
}

static void SetCh3_Set(uint16_t data)
{
	StoreConf.Ch3_Set = data;
	StoreConf.CrcVal = CheckCRC((uint8_t *)&StoreConf, sizeof(StoreConfStr) - 4);
	WriteStoreConf();
}


/**
 * @brief Modbus crc??
 * @param buf
 * @param bufLen
 * @return
 */
uint16_t Modbus_Crc_Compute(const uint8_t *buf, uint16_t bufLen) {
	uint8_t num;
	uint16_t modbus16 = UINT16_MAX;
	uint16_t index = 0;
	for (index = 0; index < bufLen; ++index) {
		num = (uint8_t) (modbus16 & UINT32_MAX);
		modbus16 = (uint16_t) (((uint32_t) modbus16 >> 8) ^ crcTb[(num ^ buf[index]) & UINT8_MAX]);
	}
	return modbus16;
}

uint16_t Modbus_Crc_Compute_cont(const uint8_t *buf, uint16_t bufLen, uint16_t val) {
	uint8_t num;
	uint16_t modbus16 = val;
	uint16_t index = 0;
	for (index = 0; index < bufLen; ++index) {
		num = (uint8_t) (modbus16 & UINT32_MAX);
		modbus16 = (uint16_t) (((uint32_t) modbus16 >> 8) ^ crcTb[(num ^ buf[index]) & UINT8_MAX]);
	}
	return modbus16;
}


static uint16_t crcTb[] = {
		0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
		0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
		0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
		0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
		0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
		0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
		0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
		0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
		0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
		0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
		0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
		0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
		0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
		0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
		0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
		0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
		0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
		0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
		0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
		0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
		0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
		0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
		0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
		0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
		0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
		0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
		0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
		0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
		0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
		0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
		0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
		0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

void modbusValInit(void)
{
		modbus.step = MODBUS_SLAVE_ADDR;
		modbus.addr = 0;
		modbus.function = 0;
		memset(modbus.data, 0, sizeof(modbus.data));
		modbus.crc = 0;
	
		modTimer.state = TIMER_DISABLE;
		modTimer.timerCount = 0;
		memset(&TempHumiData, 0xFF, sizeof(TempHumiData));
		
		memset(TempHumiData.selfTempUnix, 0, sizeof(TempHumiData.selfTempUnix));
		TempHumiData.selfHumiUnix = 0;
}

static void ModbusSend(uint8_t* buf, uint8_t len)
{
		HostUartSendBuf(buf, len);
}

static void ModbusTimerEnable(void)
{
		modTimer.state = TIMER_ENABLE;
		modTimer.timerCount = 0;
}

static void ModbusTimerDisable(void)
{
		modTimer.state = TIMER_DISABLE;
		modTimer.timerCount = 0;
}

void ModbusTimerAct(void)
{
		if(modTimer.state)
		{
				modTimer.timerCount++;
				if(modTimer.timerCount >= MODBUS_PARSE_TIEOUT)
				{
						ModbusTimerDisable();
						modbus.step = MODBUS_SLAVE_ADDR;
				}
		}
}


void modbusAct(void)
{
		uint8_t tempBuf[256] = {0};
		uint8_t i = 0, j = 0, k = 0, m = 5;
		uint16_t strtAddr = 0;
		uint16_t num = 0;
		uint16_t crc = 0;
		uint8_t FindFlag = 0;
		uint16_t tempRet = 0;
		
		strtAddr += ((uint16_t)modbus.data[0]) << 8;
		strtAddr += modbus.data[1];
		num += ((uint16_t)modbus.data[2]) << 8;
		num += modbus.data[3];
		if(modbus.function == MODBUS_READ_MUL_REG)
		{								
				if(modbus.addr == StoreConf.SlaveAddr)
				{
						for(j = 0; j < (sizeof(table) / sizeof(FunTable)); j++)
						{
								if(table[j].reg == strtAddr)
								{
										FindFlag = 1;
										break;
								}
						}
						if(FindFlag && (num <= (REG_TOTAL - strtAddr)))
						{
								tempBuf[i++] = StoreConf.SlaveAddr;
								tempBuf[i++] = MODBUS_READ_MUL_REG;
								tempBuf[i++] = num * 2;
								for(k = 0; k < num; k++)
								{
										tempRet = table[j + k].fun();
										tempBuf[i++] = tempRet >> 8;
										tempBuf[i++] = tempRet;
								}
								crc = Modbus_Crc_Compute(tempBuf, i);
								tempBuf[i++] = (uint8_t)crc;
								tempBuf[i++] = (uint8_t)(crc >> 8);
								ModbusSend(tempBuf, i);
						}
						if(strtAddr >= MODBUS_WL_TEMP_ID_START_REG && strtAddr <= MODBUS_WL_TEMP_ID_END_REG)
						{
								if(num <= (MODBUS_WL_TEMP_ID_END_REG - strtAddr + 1))
								{
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_READ_MUL_REG;
										tempBuf[i++] = num * 2;
										for(k = 0; k < num; k++)
										{
												if((strtAddr + k - MODBUS_WL_TEMP_ID_START_REG) < 90)
												{
														tempBuf[i++] = TempHumiData.tempID[strtAddr + k - MODBUS_WL_TEMP_ID_START_REG] >> 8;
														tempBuf[i++] = TempHumiData.tempID[strtAddr + k - MODBUS_WL_TEMP_ID_START_REG];
												}
												else
												{
														tempBuf[i++] = 0xFF;
														tempBuf[i++] = 0xFF;
												}
										}
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
						}
						if(strtAddr >= MODBUS_WL_HUMI_ID_START_REG && strtAddr <= MODBUS_WL_HUMI_ID_END_REG)
						{
								if(num <= (MODBUS_WL_HUMI_ID_END_REG - strtAddr + 1))
								{
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_READ_MUL_REG;
										tempBuf[i++] = num * 2;
										for(k = 0; k < num; k++)
										{
												if((strtAddr + k - MODBUS_WL_HUMI_ID_START_REG) < 15)
												{
														tempBuf[i++] = TempHumiData.humiID[strtAddr + k - MODBUS_WL_HUMI_ID_START_REG] >> 8;
														tempBuf[i++] = TempHumiData.humiID[strtAddr + k - MODBUS_WL_HUMI_ID_START_REG];
												}
												else
												{
														tempBuf[i++] = 0xFF;
														tempBuf[i++] = 0xFF;
												}
										}
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
						}
						if(strtAddr >= MODBUS_WL_TEMP_VAL_START_REG && strtAddr <= MODBUS_WL_TEMP_VAL_END_REG)
						{
								if(num <= (MODBUS_WL_TEMP_VAL_END_REG - strtAddr + 1))
								{
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_READ_MUL_REG;
										tempBuf[i++] = num * 2;
										for(k = 0; k < num; k++)
										{
												if((strtAddr + k - MODBUS_WL_TEMP_VAL_START_REG) < 90)
												{
														tempBuf[i++] = TempHumiData.tempVal[strtAddr + k - MODBUS_WL_TEMP_VAL_START_REG] >> 8;
														tempBuf[i++] = TempHumiData.tempVal[strtAddr + k - MODBUS_WL_TEMP_VAL_START_REG];
												}
												else
												{
														tempBuf[i++] = 0xFF;
														tempBuf[i++] = 0xFF;
												}
										}
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
						}
						if(strtAddr >= MODBUS_WL_HUMI_VAL_START_REG && strtAddr <= MODBUS_WL_HUMI_VAL_END_REG)
						{
								if(num <= (MODBUS_WL_HUMI_VAL_END_REG - strtAddr + 1))
								{
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_READ_MUL_REG;
										tempBuf[i++] = num * 2;
										for(k = 0; k < num; k++)
										{
												if((strtAddr + k - MODBUS_WL_HUMI_VAL_START_REG) < 15)
												{
														tempBuf[i++] = TempHumiData.humiVal[strtAddr + k - MODBUS_WL_HUMI_VAL_START_REG] >> 8;
														tempBuf[i++] = TempHumiData.humiVal[strtAddr + k - MODBUS_WL_HUMI_VAL_START_REG];
												}
												else
												{
														tempBuf[i++] = 0xFF;
														tempBuf[i++] = 0xFF;
												}
										}
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
						}
						if(strtAddr == REG_NUM_SAMPLE) {
							tempBuf[i++] = StoreConf.SlaveAddr;
							tempBuf[i++] = MODBUS_READ_MUL_REG;
							tempBuf[i++] = 2;
							tempBuf[i++] = 0x00;
							tempBuf[i++] = StoreInfo.NumberUseBlock;
							crc = Modbus_Crc_Compute(tempBuf, i);
							tempBuf[i++] = (uint8_t)crc;
							tempBuf[i++] = (uint8_t)(crc >> 8);
							ModbusSend(tempBuf, i);
						}
						if(strtAddr == REG_READ_DATA1_SIZE) {
							tempBuf[i++] = StoreConf.SlaveAddr;
							tempBuf[i++] = MODBUS_READ_MUL_REG;
							tempBuf[i++] = 4;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[0] >> 24;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[0] >> 16;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[0] >> 8;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[0];
							crc = Modbus_Crc_Compute(tempBuf, i);
							tempBuf[i++] = (uint8_t)crc;
							tempBuf[i++] = (uint8_t)(crc >> 8);
							ModbusSend(tempBuf, i);
						}
						if(strtAddr == REG_READ_DATA2_SIZE) {
							tempBuf[i++] = StoreConf.SlaveAddr;
							tempBuf[i++] = MODBUS_READ_MUL_REG;
							tempBuf[i++] = 4;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[1] >> 24;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[1] >> 16;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[1] >> 8;
							tempBuf[i++] = StoreInfo.BlockStoreBytes[1];
							crc = Modbus_Crc_Compute(tempBuf, i);
							tempBuf[i++] = (uint8_t)crc;
							tempBuf[i++] = (uint8_t)(crc >> 8);
							ModbusSend(tempBuf, i);
						}
						if(strtAddr == REG_READ_DATA1) {
							uint32_t templen = StoreInfo.BlockStoreBytes[0];
							uint32_t addr = FLASH_BLOCK0_ADDR;
							uint16_t index = 0;
							while(templen > 0) {
								i = 0;
								tempBuf[i++] = StoreConf.SlaveAddr;
								tempBuf[i++] = MODBUS_READ_MUL_REG;
								if(templen >= 200) {
									tempBuf[i++] = 202;
									tempBuf[i++] = index >> 8;
									tempBuf[i++] = index;
									ReadDatFromFlash(addr, &tempBuf[i], 200);
									i += 200;
									templen -= 200;
									addr += 200;
								}
								else {
									tempBuf[i++] = templen + 2;
									tempBuf[i++] = index >> 8;
									tempBuf[i++] = index;
									ReadDatFromFlash(addr, &tempBuf[i], templen);
									i += templen;
									addr += templen;
									templen = 0;
								}
								crc = Modbus_Crc_Compute(tempBuf, i);
								tempBuf[i++] = (uint8_t)crc;
								tempBuf[i++] = (uint8_t)(crc >> 8);
								ModbusSend(tempBuf, i);	
								index++;
							}
						}
						if(strtAddr == REG_READ_DATA2) {
							uint32_t templen = StoreInfo.BlockStoreBytes[1];
							uint32_t addr = FLASH_BLOCK1_ADDR;
							uint16_t index = 0;
							while(templen > 0) {
								i = 0;
								tempBuf[i++] = StoreConf.SlaveAddr;
								tempBuf[i++] = MODBUS_READ_MUL_REG;
								if(templen >= 200) {
									tempBuf[i++] = 202;
									tempBuf[i++] = index >> 8;
									tempBuf[i++] = index;
									ReadDatFromFlash(addr, &tempBuf[i], 200);
									i += 200;
									templen -= 200;
									addr += 200;
								}
								else {
									tempBuf[i++] = templen + 2;
									tempBuf[i++] = index >> 8;
									tempBuf[i++] = index;
									ReadDatFromFlash(addr, &tempBuf[i], templen);
									i += templen;
									addr += templen;
									templen = 0;
								}
								crc = Modbus_Crc_Compute(tempBuf, i);
								tempBuf[i++] = (uint8_t)crc;
								tempBuf[i++] = (uint8_t)(crc >> 8);
								ModbusSend(tempBuf, i);	
								index++;								
							}
						}
				}				
		}
		else
		{
				if(modbus.addr == StoreConf.SlaveAddr || modbus.addr == MODBUS_BOARD_ADDR)
				{
#if 1
						for(j = 0; j < (sizeof(Wtable) / sizeof(WriteFunTable)); j++)
						{
								if(Wtable[j].reg == strtAddr)
								{
										FindFlag = 1;
										break;
								}
						}
						if(FindFlag) //&& num <= (REG_HW_VERSION - strtAddr))
						{
								if(modbus.addr != MODBUS_BOARD_ADDR)//if addr is 0, do not response
								{
										i = 0;
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_WRITE_MUL_REG;
										tempBuf[i++] = strtAddr >> 8;
										tempBuf[i++] = strtAddr;
										tempBuf[i++] = (uint8_t)(num >> 8);
										tempBuf[i++] = (uint8_t)num;
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc >> 8;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
								for(k = 0; k < num; k++)
								{
										Wtable[j + k].fun((uint16_t)(modbus.data[m] << 8) + modbus.data[m + 1]);
										m += 2;
								}
								FindFlag = 0;
						}
						for(j = 0; j < (sizeof(WtempHumiTable) / sizeof(SelfWriteFunTable)); j++)
						{
								if(WtempHumiTable[j].reg == strtAddr)
								{
										FindFlag = 1;
										break;
								}
						}
						if(FindFlag)
						{
								if(modbus.addr != MODBUS_BOARD_ADDR)//if addr is 0, do not response
								{
										i = 0;
										tempBuf[i++] = StoreConf.SlaveAddr;
										tempBuf[i++] = MODBUS_WRITE_MUL_REG;
										tempBuf[i++] = strtAddr >> 8;
										tempBuf[i++] = strtAddr;
										tempBuf[i++] = (uint8_t)(num >> 8);
										tempBuf[i++] = (uint8_t)num;
										crc = Modbus_Crc_Compute(tempBuf, i);
										tempBuf[i++] = (uint8_t)crc >> 8;
										tempBuf[i++] = (uint8_t)(crc >> 8);
										ModbusSend(tempBuf, i);
								}
								for(k = 0; k < num; k++)
								{
										WtempHumiTable[j + k].fun((uint16_t)(modbus.data[m] << 8) + modbus.data[m + 1]);
										m += 2;
								}
						}
#endif
				}
#if 0
				WriteReg(strtAddr, num, &modbus.data[5]);
				tempBuf[i++] = StoreConf.SlaveAddr;
				tempBuf[i++] = MODBUS_WRITE_MUL_REG;
				tempBuf[i++] = (uint8_t)(num >> 8);
				tempBuf[i++] = (uint8_t)num;
				crc = Modbus_Crc_Compute(tempBuf, i);
				tempBuf[i++] = (uint8_t)crc >> 8;
				tempBuf[i++] = (uint8_t)(crc >> 8);
				ModbusSend(tempBuf, i);
#endif
		}
}

void modbusParse(uint8_t ch)
{
		static uint8_t TempBuf[256] = {0};
		static uint8_t TempBufIndex = 0;
		static uint16_t tempLen = 0;
	
		ModbusTimerEnable();
		switch(modbus.step)
		{
			case MODBUS_SLAVE_ADDR:				
				if(ch == StoreConf.SlaveAddr || ch == MODBUS_BOARD_ADDR)
				{
						modbus.step = MODBUS_FUNCTION_CODE;
						modbus.addr = ch;
						modbus.dataIndex = 0;
						tempLen = 0;
						TempBufIndex = 0;
						TempBuf[TempBufIndex++] = ch;
				}
			break;
			case MODBUS_FUNCTION_CODE:
				if(ch == MODBUS_READ_MUL_REG || ch == MODBUS_WRITE_MUL_REG)
				{
						modbus.step = MODBUS_DATA;
						modbus.function = ch;
						TempBuf[TempBufIndex++] = ch;
				}
				else
				{
						modbus.step = MODBUS_SLAVE_ADDR;
				}
			break;
			case MODBUS_DATA:
				modbus.data[modbus.dataIndex++] = ch;
				TempBuf[TempBufIndex++] = ch;
				if(modbus.function == MODBUS_READ_MUL_REG)
				{
						if(modbus.dataIndex == 4)
						{
								modbus.step = MODBUS_CRC_L;
						}
				}
				else
				{
						if(modbus.dataIndex == 5)
						{
								tempLen = modbus.data[4];
						}
						if(modbus.dataIndex > 5)
						{
								tempLen--;
								if(tempLen == 0)
								{
										modbus.step = MODBUS_CRC_L;
								}
						}
				}
				
			break;
			case MODBUS_CRC_L:
				modbus.crc = Modbus_Crc_Compute(TempBuf, TempBufIndex);
				if(((uint8_t)modbus.crc) == ch || ch == 0xff)
				{
						modbus.step = MODBUS_CRC_H;
				}
				else
				{
						modbus.step = MODBUS_SLAVE_ADDR;
				}
			break;
			case MODBUS_CRC_H:
				if(((uint8_t)(modbus.crc >> 8)) == ch || ch == 0xff)
				{
						//modbus action
						ModbusTimerDisable();
						modbusAct();
				}
				modbus.step = MODBUS_SLAVE_ADDR;
			break;
			default:
				
			break;
		}
}

static void ExitBypassParse(uint8_t ch)
{
		static uint8_t step = 0;
	
		switch(step)
		{
			case 0:
				if(ch == '*')
				{
						step++;
				}
			break;
			case 1:
				if(ch == '!')
				{
						step++;
				}
				else
				{
						step = 0;
				}
			break;
			case 2:
				if(ch == '*')
				{
						step++;
				}
				else
				{
						step = 0;
				}
			break;
			case 3:
				if(ch == '@')
				{
						step++;
				}
				else
				{
						step = 0;
				}
			break;
			case 4:
				if(ch == '*')
				{
						step++;
				}
				else
				{
						step = 0;
				}
			break;
			case 5:
				if(ch == '#')
				{
						systemFlag.bit.BypassMode = 0;
				}
				step = 0;
			break;
			default:
				
			break;
		}
}

void ModbusProtocol(void)
{
		uint8_t ch = 0;
	
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_MODBUS)
		{
				if(systemFlag.bit.BypassMode)
				{
						while(HostCheck())
						{
								ch = HostGetData();
								CommunUartSendBuf(&ch, 1);
								ExitBypassParse(ch);
						}
				}
				else
				{
						while(HostCheck())
						{
								ch = HostGetData();
								modbusParse(ch);
								ReceParse(ch);
						}
						ModbusTimerAct();
				}
		}
}
