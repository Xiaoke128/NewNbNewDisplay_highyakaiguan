#include "protocol.h"
#include "hw_uart.h"

ProStr protocol;
ProTimer Ptimer;
MainBoardInfoStr MainBoardInfo = {0};
static void ProResponse(ComEnum command, uint8_t *buf, uint8_t bufLen);
static void GetWlStatus(void);
static void GetTempID(void);
static void GetHumiID(void);
static void GetTempVal(void);
static void GetHumiVal(void);
static void SetSelfTempHumi(void);
static void SendSignalInfo(void);
static void ProAction(void);
static void PtimerEnable(void);
static void PtimerDisable(void);
static void PtimerRun(void);
static void SetAbnormal(void);
static void SendCommunictionStatus(void);
static void SendMainBoardInfo(void);

ComFunStr ProFunTable[] = {
	{COMMAND_GET_WL_STATUS,							GetWlStatus},
	{COMMAND_GET_TEMP_ID,								GetTempID},
	{COMMAND_GET_HUMI_ID,								GetHumiID},
	{COMMADN_GET_TEMP_VAL,							GetTempVal},
	{COMMAND_GET_HUMI_VAL,							GetHumiVal},
	{COMMAND_SET_SELF_TEMP_HUMI,				SetSelfTempHumi},
	{COMMAND_SEND_SIGNAL_INFO,					SendSignalInfo},
	{COMMAND_SET_ABNORMAL,							SetAbnormal},
	{COMMAND_SEND_COMMUNICATION_STATUS,	SendCommunictionStatus},
	{COMMAND_SEND_MAIN_BOARD_INFO,			SendMainBoardInfo},
};


static void ProResponse(ComEnum command, uint8_t *buf, uint8_t bufLen)
{	
		uint8_t data[256] = {0};
		uint8_t i = 0;
		uint16_t crc = 0;
		
		data[i++] = PROTOCOL_HEAD;
		data[i++] = 3 + bufLen;
		data[i++] = command | 0x80;
		data[i++] = 0x01;
		if(bufLen > 0)
		{
				memcpy(&data[i], buf, bufLen);
				i += bufLen;
		}
		crc = Modbus_Crc_Compute(data, i);
		data[i++] = crc >> 8;
		data[i++] = crc;
		data[i++] = PROTOCOL_TAIL;
		CommunUartSendBuf(data, i);
}

static void GetWlStatus(void)
{
		uint8_t buf[1] = {0};
		
		buf[0] = wlStrData.sysFlag.bit.Connected;
		ProResponse(COMMAND_GET_WL_STATUS, buf, sizeof(buf));
}

static void GetTempID(void)
{
		uint8_t buf[181] = {0};
		uint8_t i = 0, j = 0;
		
		buf[j++] = wlStrData.sysFlag.bit.TempIDGet;
		for(i = 0; i < 90; i++)
		{
				buf[j++] = wlStrData.tempID_Val[i][0] >> 8;
				buf[j++] = wlStrData.tempID_Val[i][0];
		}
		ProResponse(COMMAND_GET_TEMP_ID, buf, sizeof(buf));
}

static void GetHumiID(void)
{
		uint8_t buf[31] = {0};
		uint8_t i = 0, j = 0;
		
		buf[j++] = wlStrData.sysFlag.bit.HumiIDGet;
		for(i = 0; i < 15; i++)
		{
				buf[j++] = wlStrData.humiID_Val[i][0] >> 8;
				buf[j++] = wlStrData.humiID_Val[i][0];
		}
		ProResponse(COMMAND_GET_HUMI_ID, buf, sizeof(buf));
}

static void GetTempVal(void)
{
		uint8_t buf[180] = {0};
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < 90; i++)
		{
				buf[j++] = wlStrData.tempID_Val[i][1] >> 8;
				buf[j++] = wlStrData.tempID_Val[i][1];
		}
		ProResponse(COMMADN_GET_TEMP_VAL, buf, sizeof(buf));
}

static void GetHumiVal(void)
{
		uint8_t buf[30] = {0};
		uint8_t i = 0, j = 0;
		
		for(i = 0; i < 15; i++)
		{
				buf[j++] = wlStrData.humiID_Val[i][1] >> 8;
				buf[j++] = wlStrData.humiID_Val[i][1];
		}
		ProResponse(COMMAND_GET_HUMI_VAL, buf, sizeof(buf));
}

static void SetSelfTempHumi(void)
{
		uint8_t tempMask = protocol.data[0];
		uint8_t i = 1;
		int16_t temp = 0;
	
		if(tempMask & 0x01)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[0] = temp / 10;
				DisplayData.currentTempUnix[0] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[0] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[0] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[0] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[0][0] == 0)
				{
						DispalyTempStoreAdd(0, DisplayData.currentTemp[0], DisplayData.currentTempUnix[0]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[0]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[0], DisplayData.currentTempUnix[0]);
				}
		}
		if(tempMask & 0x02)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[1] = temp / 10;
				DisplayData.currentTempUnix[1] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[1] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[1] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[1] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[1][0] == 0)
				{
						DispalyTempStoreAdd(1, DisplayData.currentTemp[1], DisplayData.currentTempUnix[1]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[1]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[1], DisplayData.currentTempUnix[1]);
				}
		}
		if(tempMask & 0x04)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[2] = temp / 10;
				DisplayData.currentTempUnix[2] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[2] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[2] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[2] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[2][0] == 0)
				{
						DispalyTempStoreAdd(2, DisplayData.currentTemp[2], DisplayData.currentTempUnix[2]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[2]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[2], DisplayData.currentTempUnix[2]);
				}
		}
		if(tempMask & 0x08)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[3] = temp / 10;
				DisplayData.currentTempUnix[3] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[3] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[3] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[3] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[3][0] == 0)
				{
						DispalyTempStoreAdd(3, DisplayData.currentTemp[3], DisplayData.currentTempUnix[3]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[3]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[3], DisplayData.currentTempUnix[3]);
				}
		}
		if(tempMask & 0x10)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[4] = temp / 10;
				DisplayData.currentTempUnix[4] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[4] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[4] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[4] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[4][0] == 0)
				{
						DispalyTempStoreAdd(4, DisplayData.currentTemp[4], DisplayData.currentTempUnix[4]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[4]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[4], DisplayData.currentTempUnix[4]);
				}
		}
		if(tempMask & 0x20)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentTemp[5] = temp / 10;
				DisplayData.currentTempUnix[5] = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentTempUnix[5] += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentTempUnix[5] += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentTempUnix[5] += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfTempIndex[5][0] == 0)
				{
						DispalyTempStoreAdd(5, DisplayData.currentTemp[5], DisplayData.currentTempUnix[5]);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentTemp[5]) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentTemp[5], DisplayData.currentTempUnix[5]);
				}
		}
		if(tempMask & 0x40)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentHumi = temp / 10;
				DisplayData.currentHumiUnix = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentHumiUnix += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentHumiUnix += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentHumiUnix += (uint32_t)(protocol.data[i++]);
				if(DisplayData.selfHumiIndex == 0)
				{
						DispalyHumiStoreAdd(DisplayData.currentHumi, DisplayData.currentHumiUnix);
				}
				if((DisplayData.abnormalHumiVal < DisplayData.currentHumi) && (DisplayData.abnormalHumiVal != 0))
				{
						DisalyAbnormalHumiAdd(DisplayData.currentHumi, DisplayData.currentHumiUnix);
				}
		}
		if(tempMask & 0x80)
		{				
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentInterTemp = temp / 10;
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentCj2301Temp = temp / 10;
				temp = (int16_t)(protocol.data[i++] << 8);
				temp += protocol.data[i++];
				DisplayData.currentCj2301Humi = temp / 10;
				DisplayData.currentInterTempUnix = (uint32_t)(protocol.data[i++] << 24);
				DisplayData.currentInterTempUnix += (uint32_t)(protocol.data[i++] << 16);
				DisplayData.currentInterTempUnix += (uint32_t)(protocol.data[i++] << 8);
				DisplayData.currentInterTempUnix += (uint32_t)(protocol.data[i++]);
				
				if(DisplayData.InterTempIndex == 0 && DisplayData.currentCj2301Temp != 0 && DisplayData.currentCj2301Humi != 0)
				{
						DisplayInterTempAdd(DisplayData.currentCj2301Temp, DisplayData.currentCj2301Humi, DisplayData.currentInterTempUnix);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentInterTemp) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentInterTemp, DisplayData.currentInterTempUnix);
				}
				if((DisplayData.abnormalTempVal < DisplayData.currentCj2301Temp) && (DisplayData.abnormalTempVal != 0))
				{
						DisplayAnnormalTempAdd(1, DisplayData.currentCj2301Temp, DisplayData.currentInterTempUnix);
				}
		}
		ProResponse(COMMAND_SET_SELF_TEMP_HUMI, NULL, 0);
}

static void SendSignalInfo(void)
{
		uint8_t signalType = protocol.data[0];
		uint16_t signalTime = (uint16_t)(protocol.data[1] << 8) + protocol.data[2];
		uint32_t signalUnix = (uint32_t)(protocol.data[3] << 24) + (uint32_t)(protocol.data[4] << 16) + (uint32_t)(protocol.data[5] << 8) + protocol.data[6];
	
		DisplaySignalStoreAdd(signalType, signalTime, signalUnix);
		ProResponse(COMMAND_SEND_SIGNAL_INFO, NULL, 0);
		if(signalType == 0)
		{
				if(DisplayData.SignalCloseAbnormalVal < signalTime && (DisplayData.SignalCloseAbnormalVal != 0))
				{
						DisplayAbnormalSignalAdd(signalType, signalTime, signalUnix);
				}
		}
		else
		{
				if(DisplayData.SignalOpenAbnormalVal < signalTime && (DisplayData.SignalOpenAbnormalVal != 0))
				{
						DisplayAbnormalSignalAdd(signalType, signalTime, signalUnix);
				}
		}
}

static void SetAbnormal(void)
{
		DisplayData.abnormalTempVal = protocol.data[0];
		DisplayData.abnormalHumiVal = protocol.data[1];
		DisplayData.SignalCloseAbnormalVal= (uint16_t)(protocol.data[2] << 8) + protocol.data[3];
		DisplayData.SignalOpenAbnormalVal = (uint16_t)(protocol.data[4] << 8) + protocol.data[5];
		ProResponse(COMMAND_SET_ABNORMAL, NULL, 0);
}

static void SendCommunictionStatus(void)
{
		systemFlag.bit.Communicate = 1;
		ProResponse(COMMAND_SEND_COMMUNICATION_STATUS, NULL, 0);
}

static void SendMainBoardInfo(void)
{
		MainBoardInfo.slaveID = protocol.data[0];
		MainBoardInfo.baudrate = (uint32_t)(protocol.data[1] << 24) + (uint32_t)(protocol.data[2] << 16) + (uint32_t)(protocol.data[3] << 8) + (uint32_t)(protocol.data[4] << 0);
		memcpy(MainBoardInfo.IMEI, &protocol.data[5], 15);
		memcpy(MainBoardInfo.ver, &protocol.data[20], 6);
		MainBoardInfo.unix = (uint32_t)(protocol.data[26] << 24);
		MainBoardInfo.unix += (uint32_t)(protocol.data[27] << 16);
		MainBoardInfo.unix += (uint32_t)(protocol.data[28] << 8);
		MainBoardInfo.unix += (uint32_t)(protocol.data[29] << 0);
		MainBoardInfo.NetLedStatus = protocol.data[30];
		MainBoardInfo.DataLedStatus = protocol.data[31];
		ProResponse(COMMAND_SEND_MAIN_BOARD_INFO, NULL, 0);
		if(StoreConf.SlaveAddr != MainBoardInfo.slaveID)
		{
				StoreConf.SlaveAddr = MainBoardInfo.slaveID;
				StoreConfStore();
		}
}

void ProParse(uint8_t ch)
{
		static uint8_t tempLen = 0;
		static uint8_t tempBuf[200] = {0};
		static uint8_t tempBufLen = 0;
		static uint16_t tempCrc = 0;
	
		PtimerEnable();
		switch(protocol.step)
		{
			case PRO_HEAD:
				protocol.len = 0;
				protocol.dataIndex = 0;
				tempLen = 0;
				if(ch == PROTOCOL_HEAD)
				{
						protocol.step = PRO_LEN;
						tempBufLen = 0;
						tempBuf[tempBufLen++] = ch;
				}
			break;
			case PRO_LEN:
				if(ch >= 3)
				{
						protocol.len = ch;
						tempLen = ch - 3;
						protocol.step = PRO_COMMAND;
						tempBuf[tempBufLen++] = ch;
				}
				else
				{
						protocol.step = PRO_HEAD;
				}
			break;
			case PRO_COMMAND:
				protocol.command = (ComEnum)ch;
				protocol.step = PRO_COM_VERSION;
				tempBuf[tempBufLen++] = ch;
			break;
			case PRO_COM_VERSION:
				protocol.comVer = ch;	
				tempBuf[tempBufLen++] = ch;
				if(protocol.len == 3)
				{
						protocol.step = PRO_CRC_H;
				}
				else
				{
						protocol.step = PRO_DATA;
				}
			break;
			case PRO_DATA:
				protocol.data[protocol.dataIndex++] = ch;
				tempBuf[tempBufLen++] = ch;
				if((--tempLen) == 0)
				{
						protocol.step = PRO_CRC_H;
				}
			break;
			case PRO_CRC_H:
				tempCrc = Modbus_Crc_Compute(tempBuf, tempBufLen);
				if(ch == (uint8_t)(tempCrc >> 8))
				{
					protocol.step = PRO_CRC_L;
				}
				else
				{
					protocol.step = PRO_HEAD;
				}
			break;
			case PRO_CRC_L:
				if(ch == (uint8_t)tempCrc)
				{
					protocol.step = PRO_TAIL;
				}
				else
				{
					protocol.step = PRO_HEAD;
				}
			break;
			case PRO_TAIL:
				if(ch == PROTOCOL_TAIL)
				{
						//add command action
						systemFlag.bit.Communicate = 1;
						PtimerDisable();
						ProAction();
				}
				protocol.step = PRO_HEAD;
			break;
			default:
				
			break;
		}
}

static void ProAction(void)
{
		uint8_t i = 0;
	
		for(i = 0; i < sizeof(ProFunTable) / sizeof(ComFunStr); i++)
		{
				if(ProFunTable[i].command == protocol.command)
				{
						ProFunTable[i].fun();
						break;
				}
		}
}


static void PtimerEnable(void)
{
		Ptimer.enable = 1;
		Ptimer.count = 0;
}

static void PtimerDisable(void)
{
		Ptimer.enable = 0;
		Ptimer.count = 0;
}

static void PtimerRun(void)
{
		if(Ptimer.enable)
		{
				Ptimer.count++;
				if(Ptimer.count >= 300)
				{
						Ptimer.count = 0;
						protocol.step = PRO_HEAD;
						Ptimer.enable = 0;
				}
		}
}

static void CheckTimeStore(void)
{
		static uint32_t count = 0;
	
		count++;
		if(count >= (24 * 60 * 60 * 1000))//(24 * 60 * 60 * 1000)
		{
				count = 0;
				DispalyTempStoreAdd(0, DisplayData.currentTemp[0], DisplayData.currentTempUnix[0]);
				DispalyTempStoreAdd(1, DisplayData.currentTemp[1], DisplayData.currentTempUnix[1]);
				DispalyTempStoreAdd(2, DisplayData.currentTemp[2], DisplayData.currentTempUnix[2]);
				DispalyTempStoreAdd(3, DisplayData.currentTemp[3], DisplayData.currentTempUnix[3]);
				DispalyTempStoreAdd(4, DisplayData.currentTemp[4], DisplayData.currentTempUnix[4]);	
				DispalyTempStoreAdd(5, DisplayData.currentTemp[5], DisplayData.currentTempUnix[5]);
				DispalyHumiStoreAdd(DisplayData.currentHumi, DisplayData.currentHumiUnix);
				DisplayInterTempAdd(DisplayData.currentCj2301Temp, DisplayData.currentCj2301Humi, DisplayData.currentInterTempUnix);
		}
}

void DisplayMainProInit(void)
{
		memset(&protocol, 0, sizeof(protocol));
		memset(&Ptimer, 0, sizeof(Ptimer));
}

static void NetLedFlash(void)
{
		static uint16_t count = 0;
		static uint16_t temp = 0;
		static uint8_t ledStatus = 0;
		
		if(MainBoardInfo.NetLedStatus == NET_LED_STATUS_FLASH_200MS)
		{
				temp = 200;
		}
		else
		{
				temp = 1000;
		}
		count++;
		if(count >= temp)
		{
				count = 0;
				if(ledStatus)
				{
						NET_LED_OFF;						
				}
				else
				{
						NET_LED_ON;
				}
				ledStatus = !ledStatus;
		}
}

static void DataAbnormalFlash(void)
{
		static uint16_t count = 0;
		static uint8_t ledStatus = 0;
	
		count++;
		if(count >= 500)
		{
				count = 0;
				if(ledStatus)
				{
						DATA_LED_ABNORMAL_ON;						
				}
				else
				{
						DATA_LED_ABNORMAL_OFF;
				}
				ledStatus = !ledStatus;
		}
}

static void LedControl(void)
{
		if(MainBoardInfo.NetLedStatus == NET_LED_STATUS_OFF)
		{
				NET_LED_OFF;
		}
		else
		{
				NetLedFlash();
		}
		if(MainBoardInfo.DataLedStatus == DATA_LED_STATUS_NORMAL)
		{
				DATA_LED_NORMAL;
		}
		else if(MainBoardInfo.DataLedStatus == DATA_LEN_STATUS_ALARM)
		{
				DATA_LED_ALARM;
		}
		else
		{
				DataAbnormalFlash();
		}
}

void DisplayMainProTask(void)
{
		uint8_t ch = 0;
		
		CheckTimeStore();
		PtimerRun();
		PakTimerRun();
		if(CommunCheck())
		{
				ch = CommunGetData();
				ProParse(ch);
				ReceParse(ch);
		}
		LedControl();
}
