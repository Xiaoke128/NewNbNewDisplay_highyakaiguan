#include "wh_nb73.h"

static void ResBuffAdd(void);
static void NbReset(void);
static void GetImeiFunc(void);
static void RunGetImei(void);
static void GetImeiDisable(void);
static void GetImei(void);
static AT_ResType ParseGetImei(void);

static void GetCcidFunc(void);
static void RunGetCCID(void);
static void GetCCIDDisable(void);
static void GetCCID(void);
static AT_ResType ParseGetCCID(void);

static void NbResetFunc(void);
static void NbResetDisable(void);

static void GetPciDisable(void);
static AT_ResType ParseGetPCI(void);
static void GetPCI(void);
static void RunGetPCI(void);
static void GetPciFunc(void);

static AT_ResType ParseGetAttach(void);
static void GetAttach(void);
static void GetAttachDisable(void);
static void RunGetAttach(void);
static void GetAttachFunc(void);

static AT_ResType ParseGetRSSI(void);
static void GetRSSI(void);
static void GetRssiDisable(void);
static void RunGetRSSI(void);
static void GetRssiFunc(void);

static void CreateSocketFunc(void);
static AT_ResType ParseCreateSocket(void);
static void CreateSocket(void);
static void CreateSocketDisable(void);
static void RunCreateSocket(void);

static void SendDataFunc(void);
static void RunSendData(void);
static void SendDataDisable(void);
static void SenData(void);
static AT_ResType ParseSendData(void);

static void ReadDataFunc(void);
static void RunReadData(void);
static void ReadDataDisable(void);
static void ReadData(void);
static AT_ResType ParseReadData(void);

static void GetIpFunc(void);
static void RunGetIp(void);
static void GetIpDisable(void);
static void GetIp(void);
static AT_ResType ParseGetIp(void);

static AT_ResType NbParse(void);

static void RevDataRead(void);

static void ClearRevBuf(void);
NbInfomation NbInfo;
StoreConfStr StoreConf;

static uint8_t ResDataBuf[1024];
static uint16_t ResDataIndex = 0;

static char sendBuf[255] = "AT+SKTSEND=1,";
//static char sendBuf[255] = "AT+NSOST=1,10.26.10.54,1883,";
static uint8_t SendBufIndex = 0;

static uint8_t RevBuf[100];
static uint8_t RevBufIndex = 0;

#define AT_FUNC_TABLE_COUNT				10

//NB AT command application
static void (*AT_FuncTable[AT_FUNC_TABLE_COUNT])(void) = 
{
	NbResetFunc,
	GetImeiFunc,
	GetCcidFunc,
	CreateSocketFunc,//change AT+ECICCID
	GetPciFunc,//AT+ECSTATUS
	GetAttachFunc,
	GetRssiFunc,
	SendDataFunc,
	ReadDataFunc,
	GetIpFunc,
};
//byte change to hex string
static void Byte2HexString(char des[2], uint8_t source)
{
	uint8_t t1, t2;
	
	t1 = source / 16;
	t2 = source % 16;
	if(t1 <= 9)
	{
		des[0] = t1 + '0';//0 ~ 9
	}
	else
	{
		des[0] = t1 + 55;//A ~ F
	}
	if(t2 <= 9)
	{
		des[1] = t2 + '0';//0 ~ 9
	}
	else
	{
		des[1] = t2 + 55;//A ~ F
	}
}
//IMEI bytes change to BCD
static void IMEI_TO_BCD(void)
{
	uint8_t i = 0, j = 0;
	
	for(i = 0; i < sizeof(NbInfo.IMEI_BCD); i++)
	{
		NbInfo.IMEI_BCD[i] = ((NbInfo.IMEI_BYTE[j] & 0x0F) << 4) + (NbInfo.IMEI_BYTE[j + 1] & 0x0F);
		j += 2;
	}
}
//CCID bytes change to BCD
static void CCID_TO_BCD(void)
{
	uint8_t i = 0, j = 0;
	
	for(i = 0; i < sizeof(NbInfo.CCID_BCD); i++)
	{
		NbInfo.CCID_BCD[i] = ((NbInfo.CCID_BYTE[j] & 0x0F) << 4) + (NbInfo.CCID_BYTE[j + 1] & 0x0F);
		j += 2;
	}
}
//get the check sum
static uint8_t GetCheckSum(uint8_t *buf, uint8_t len)
{
	uint8_t i = 0;
	uint8_t res = 0;
	
	for(i = 0; i < len; i++)
	{
		res += buf[i];
	}
	return res;
}
//read data(hex string) change to byte buffer and parse command
static void ReadData2Byte(char *buf, uint8_t len)
{
	uint8_t j = 0;
	uint8_t temp1 = 0, temp2 = 0;
	uint8_t crc = 0;
	
	for(RevBufIndex = 0; RevBufIndex < len; RevBufIndex++)
	{
		if(buf[j] < 58)
		{
			temp1 = buf[j] - '0';
		}
		else if(buf[j] < 71)
		{
			temp1 = buf[j] - 55;
		}
		else 
		{
			temp1 = buf[j] - 87;
		}
		j++;
		if(buf[j] < 58)
		{
			temp2 = buf[j] - '0';
		}
		else if(buf[j] < 71)
		{
			temp2 = buf[j] - 55;
		}
		else 
		{
			temp2 = buf[j] - 87;
		}
		j++;
		RevBuf[RevBufIndex] = ((temp1 & 0x0F) << 4) + (temp2 & 0x0F);
	}
	crc = GetCheckSum(RevBuf, (RevBufIndex - 1));
	if(RevBuf[RevBufIndex - 1] == crc)//check checksum
	{
		//command check
		if(RevBuf[RES_DATA_COMMAND_TYPE_OFFSET] == (SEND_REGISTER_BUF | RES_COMMAND_TYPE_MASK))//resgister response
		{
			NbInfo.InfoFlags.bit.CheckRegRes = 0;
			NbInfo.InfoFlags.bit.RefisterSuccess = 1;
			NbInfo.uTime = (uint32_t)RevBuf[RES_UNIX_TIME_OFFSET];
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 1] << 8);
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 2] << 16);
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 3] << 24);
			Unix_to_UTC(NbInfo.uTime, &TimeStr);
			RtcSetTime(TimeStr);
		}
		else if(RevBuf[RES_DATA_COMMAND_TYPE_OFFSET] == SEND_REVEIVE_BUF)//configure package
		{
			//Tepture collect interval
			NbInfo.ConfData.TemCollectInterval = (uint16_t)RevBuf[COLLECT_INTERVAL_OFFSET];
			NbInfo.ConfData.TemCollectInterval += (uint16_t)(RevBuf[COLLECT_INTERVAL_OFFSET + 1] << 8);
			//Time tick
			NbInfo.ConfData.TimeTick = (uint16_t)RevBuf[TIME_TICK_OFFSET];
			NbInfo.ConfData.TimeTick += (uint16_t)(RevBuf[TIME_TICK_OFFSET + 1] << 8);
			//report peroid
			NbInfo.ConfData.ReportInterval = RevBuf[REPORT_PERIOD_OFFSET];
			//Close alarm time
			NbInfo.ConfData.CloseAlramTime = (uint32_t)(RevBuf[CLOSE_ALARM_OFFSET]);
			NbInfo.ConfData.CloseAlramTime += (uint32_t)(RevBuf[CLOSE_ALARM_OFFSET + 1] << 8);
			NbInfo.ConfData.CloseAlramTime += (uint32_t)(RevBuf[CLOSE_ALARM_OFFSET + 2] << 16);
			NbInfo.ConfData.CloseAlramTime += (uint32_t)(RevBuf[CLOSE_ALARM_OFFSET + 3] << 24);
			//Open alarm time
			NbInfo.ConfData.OpenAlarmTime = (uint32_t)(RevBuf[OPEN_ALARM_OFFSET]);
			NbInfo.ConfData.OpenAlarmTime += (uint32_t)(RevBuf[OPEN_ALARM_OFFSET + 1] << 8);
			NbInfo.ConfData.OpenAlarmTime += (uint32_t)(RevBuf[OPEN_ALARM_OFFSET + 2] << 16);
			NbInfo.ConfData.OpenAlarmTime += (uint32_t)(RevBuf[OPEN_ALARM_OFFSET + 3] << 24);
			//Close abnormal time
			NbInfo.ConfData.AbnormalCloseTime = (uint32_t)(RevBuf[CLOSE_ABNORMAL_OFFSET]);
			NbInfo.ConfData.AbnormalCloseTime += (uint32_t)(RevBuf[CLOSE_ABNORMAL_OFFSET + 1] << 8);
			NbInfo.ConfData.AbnormalCloseTime += (uint32_t)(RevBuf[CLOSE_ABNORMAL_OFFSET + 2] << 16);
			NbInfo.ConfData.AbnormalCloseTime += (uint32_t)(RevBuf[CLOSE_ABNORMAL_OFFSET + 3] << 24);
			//Open abnormal time
			NbInfo.ConfData.AbnormalOpenTime = (uint32_t)(RevBuf[OPEN_ABNORMAL_OFFSET]);
			NbInfo.ConfData.AbnormalOpenTime += (uint32_t)(RevBuf[OPEN_ABNORMAL_OFFSET + 1] << 8);
			NbInfo.ConfData.AbnormalOpenTime += (uint32_t)(RevBuf[OPEN_ABNORMAL_OFFSET + 2] << 16);
			NbInfo.ConfData.AbnormalOpenTime += (uint32_t)(RevBuf[OPEN_ABNORMAL_OFFSET + 3] << 24);
			//Tempture alarm value
			NbInfo.ConfData.AbnormalTem = RevBuf[TEMP_ALARM_OFFSET] - 100;
			NbInfo.ConfData.AbnormalHumi = RevBuf[HUMI_ALARM_OFFSET];
			NbInfo.ConfData.TempDiffAlarmVal = RevBuf[TEMP_DIFF_OFFSET];
			//update report start time
			NbInfo.ReportStartUtime = UTC_to_Unix(TimeStr) + (uint32_t)(NbInfo.ConfData.TimeTick * 60);//report start time
			//set the flag to send response
			NbInfo.InfoFlags.bit.ConfResSend = 1;
			StoreConfUpdate();
			if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
			{
					TX_ENABLE;
					printf("Tempure collet interval:%d s\r\nTime Tick:%d m\r\nReport interval:%d h\r\nClose alarm time:%d\r\nOpen alarm:%d\r\nTemputre alarm:%d\r\nTemputre diff alarm:%d", NbInfo.ConfData.TemCollectInterval, NbInfo.ConfData.TimeTick, NbInfo.ConfData.ReportInterval,
					NbInfo.ConfData.CloseAlramTime, NbInfo.ConfData.OpenAlarmTime, NbInfo.ConfData.AbnormalTem, NbInfo.ConfData.TempDiffAlarmVal);
			}
		}
		else if(RevBuf[RES_DATA_COMMAND_TYPE_OFFSET] == (SEND_DATA_BUF | RES_COMMAND_TYPE_MASK))//data package response
		{
			if(NbInfo.SendPackNum > 0)
			{
				NbInfo.SendPackNum--;
				StoreInfo.NeedSendNum = NbInfo.SendPackNum;
				StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
				WriteStoreInfo();
			}
			if(NbInfo.AlarmFlags.val)
			{
				NbInfo.AlarmSendOut = 1;
			}
			NbInfo.InfoFlags.bit.CheckDataRes = 0;
			systemFlag.bit.SendSuccess = 1;
			if(systemFlag.bit.BackUpSend)
			{
				systemFlag.bit.BackUpSend = 0;
				BackUpClearOne();
			}
			NbInfo.Chanel4_AlarmFlag = 0x00;
			NbInfo.Stype = SIGNAL_NO;//clear open or close knife
			NbInfo.Stime = 0;
			NbInfo.uTime = (uint32_t)RevBuf[RES_UNIX_TIME_OFFSET];
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 1] << 8);
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 2] << 16);
			NbInfo.uTime += (uint32_t)(RevBuf[RES_UNIX_TIME_OFFSET + 3] << 24);
			Unix_to_UTC(NbInfo.uTime, &TimeStr);
			RtcSetTime(TimeStr);
		}
	}
}
//send data package build
static void SnedBufBuild(void)
{
	uint8_t buf[100] = {0};
	uint8_t index = 0;
	uint8_t i = 0;
	
	sendBuf[11] = NbInfo.Socket;
	buf[index++] = 0x01;//protocol ver
	buf[index++] = 0x01;//Device type
	memcpy(&buf[index], NbInfo.IMEI_BCD, sizeof(NbInfo.IMEI_BCD));
	index += sizeof(NbInfo.IMEI_BCD);//IMEI
	buf[index++] = NbInfo.Btype;//DATA type
	index += 5;//reserved 5 bytes
	buf[index++] = 0x01;//Hw Ver
	buf[index++] = 0x01;//Sw Ver
	buf[index++] = NbInfo.RSSI;
	index += 5;//reserved 5 bytes

	SendBufIndex = 13;
	//SendBufIndex = 30;
	if(NbInfo.Btype == SEND_REGISTER_BUF)//register package
	{
		sendBuf[SendBufIndex++] = '4';//send data size is 45 bytes
		sendBuf[SendBufIndex++] = '5';
		//sendBuf[28] = '4';
		//sendBuf[29] = '5';
		sendBuf[SendBufIndex++] = ',';
		buf[index++] = 0x12;//data size 18 bytes
		buf[index++] = 0x00;
		Isl1208GetTime();
		buf[index++] = (uint8_t)NbInfo.uTime;//unix time
		buf[index++] = (uint8_t)(NbInfo.uTime >> 8);
		buf[index++] = (uint8_t)(NbInfo.uTime >> 16);
		buf[index++] = (uint8_t)(NbInfo.uTime >> 24);
		buf[index++] = (uint8_t)NbInfo.ConfData.TimeTick;//current tick
		buf[index++] = (uint8_t)(NbInfo.ConfData.TimeTick >> 8);
		buf[index++] = (uint8_t)NbInfo.PCI;//PCI
		buf[index++] = (uint8_t)(NbInfo.PCI >> 8);
		memcpy(&buf[index], NbInfo.CCID_BCD, sizeof(NbInfo.CCID_BCD));//CCID
		index += sizeof(NbInfo.CCID_BCD);
		buf[index] = GetCheckSum(buf, index);
		index++;
		for(i = 0; i < index; i++)
		{
			Byte2HexString(&sendBuf[SendBufIndex], buf[i]);
			SendBufIndex += 2;
		}
	}
	else if(NbInfo.Btype == SEND_DATA_BUF)//data package
	{
		sendBuf[SendBufIndex++] = '4';//send data size is 40 bytes
		sendBuf[SendBufIndex++] = '0';
		//sendBuf[28] = '3';//send data size is 38 bytes
		//sendBuf[29] = '8';
		sendBuf[SendBufIndex++] = ',';
		buf[index++] = 0x0D;//data size 13 bytes
		buf[index++] = 0x00;
		if(systemFlag.bit.BackUpSend)
		{
			buf[index++] = (uint8_t)BackUpInfo.Info[0].UnixTime;
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].UnixTime >> 8);
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].UnixTime >> 16);
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].UnixTime >> 24);
			buf[index++] = BackUpInfo.Info[0].InterTemp;
			buf[index++] = BackUpInfo.Info[0].ExterTemp;
			buf[index++] = 0x00;//reserved for humidity value
			buf[index++] = BackUpInfo.Info[0].Ch4_Alarm;//channel 4 alarm flag
			buf[index++] = BackUpInfo.Info[0].SignalType;
			buf[index++] = (uint8_t)BackUpInfo.Info[0].SignalTime;
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].SignalTime >> 8);
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].SignalTime >> 16);
			buf[index++] = (uint8_t)(BackUpInfo.Info[0].SignalTime >> 24);
		}
		else
		{
			if(NbInfo.SendPackNum > 0)
			{
				buf[index++] = (uint8_t)StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime;
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime >> 8);
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime >> 16);
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].UnixTime >> 24);
				buf[index++] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].InterTemp;
				buf[index++] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].ExterTemp;
				buf[index++] = 0x00;//reserved for humidity value
				buf[index++] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].Ch4_Alarm;//channel 4 alarm flag
				buf[index++] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalType;
				buf[index++] = (uint8_t)StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime;
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime >> 8);
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime >> 16);
				buf[index++] = (uint8_t)(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum].SignalTime >> 24);
			}
		}
		buf[index] = GetCheckSum(buf, index);//check sum
		index++;
		for(i = 0; i < index; i++)
		{
			Byte2HexString(&sendBuf[SendBufIndex], buf[i]);//bytes change to hex string
			SendBufIndex += 2;
		}
	}
	else if(NbInfo.Btype == SEND_REVEIVE_BUF)
	{
		index = 16;
		sendBuf[SendBufIndex++] = '2';//send data size is 38 bytes
		sendBuf[SendBufIndex++] = '4';
		//sendBuf[28] = '2';//send data size is 38 bytes
		//sendBuf[29] = '4';
		sendBuf[SendBufIndex++] = ',';
		buf[10] = NbInfo.Btype | RES_COMMAND_TYPE_MASK;//DATA type
		buf[index++] = 0x05;//data size 5 bytes
		buf[index++] = 0x00;
		buf[index++] = 0x00;
		Isl1208GetTime();
		buf[index++] = (uint8_t)NbInfo.uTime;//unix time
		buf[index++] = (uint8_t)(NbInfo.uTime >> 8);
		buf[index++] = (uint8_t)(NbInfo.uTime >> 16);
		buf[index++] = (uint8_t)(NbInfo.uTime >> 24);
		buf[index] = GetCheckSum(buf, index);//check sum
		index++;
		for(i = 0; i < index; i++)
		{
			Byte2HexString(&sendBuf[SendBufIndex], buf[i]);//bytes change to hex string
			SendBufIndex += 2;
		}
	}
	sendBuf[SendBufIndex++] = '\r';
	sendBuf[SendBufIndex++] = '\n';
}
//enable get IMEI
void GetImeiEnable(void)
{
	NbInfo.AppPro.bit.GetImei = 1;
}

static void GetImeiFunc(void)
{
	if(NbInfo.AppPro.bit.GetImei)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_IMEI))
		{
			RunGetImei();
		}
	}
}

static void RunGetImei(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	static uint8_t rty = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0://send AT command
		GetImei();
		NbInfo.RunInStep = RUN_IN_GET_IMEI;
		step++;
		break;
	case 1://parse AT command
		res = ParseGetImei();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2://delay 5 seconds, rty 3 times, if error, reset NB module
		count++;
		if(count >= (5 * 1000))
		{
			count = 0;
			rty++;
			if(rty >= 3)
			{
				rty = 0;
				GetImeiDisable();
				NbResetEnable();
			}
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void GetImeiDisable(void)
{
	NbInfo.AppPro.bit.GetImei = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

static void GetImei(void)
{
	char buf[] = "AT+CGSN=1\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetImei(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	uint8_t i = 0;
	static uint16_t count = 0;
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))
	{
		count = 0;
		NbInfo.InfoFlags.bit.IMEI_Get = 1;
		for(i = 0; i < ResDataIndex; i++)
		{
			if((ResDataBuf[i] >= '1') && (ResDataBuf[i] <= '9'))
			{
				break;
			}
		}
		NbInfo.IMEI_BYTE[0] = '0';
		memcpy(&NbInfo.IMEI_BYTE[1], &ResDataBuf[i], 15);//copy IMEI
		IMEI_TO_BCD();
		ResType = AT_RES_OK;
		GetImeiDisable();
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)//timeout 500ms
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void GetCcidEnable(void)
{
	NbInfo.AppPro.bit.GetCCID = 1;
}

static void GetCcidFunc(void)
{
	if(NbInfo.AppPro.bit.GetCCID)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_CCID))
		{
			RunGetCCID();
		}
	}
}

static void RunGetCCID(void)
{
	static uint8_t step = 0;
	AT_ResType res;
	static uint16_t count = 0;
	
	switch(step)
	{
	case 0://send AT command
		GetCCID();
		NbInfo.RunInStep = RUN_IN_GET_CCID;
		step++;
		break;
	case 1://Parse AT command
		res = ParseGetCCID();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2://delay 5 seconds and check again
		count++;
		if(count >= 5000)
		{
			count = 0;
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void GetCCIDDisable(void)
{
	NbInfo.AppPro.bit.GetCCID = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

static void GetCCID(void)
{
	char buf[] = "AT+ECICCID\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetCCID(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0;
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))//response ok
	{
		count = 0;
		NbInfo.InfoFlags.bit.CCID_Get = 1;
		for(i = 0; i < ResDataIndex; i++)
		{
			if((ResDataBuf[i] >= '0') && (ResDataBuf[i] <= '9'))
			{
				break;
			}
		}
		memcpy(NbInfo.CCID_BYTE, &ResDataBuf[i], 20);//copy CCID
		CCID_TO_BCD();
		ResType = AT_RES_OK;
		GetCCIDDisable();
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void GetPciEnable(void)
{
	NbInfo.AppPro.bit.GetPCI = 1;
}

static void GetPciFunc(void)
{
	if(NbInfo.AppPro.bit.GetPCI)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_PCI))
		{
			RunGetPCI();
		}
	}
}

static void RunGetPCI(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0://send AT command
		GetPCI();
		NbInfo.RunInStep = RUN_IN_GET_PCI;
		step++;
		break;
	case 1://parse AT command
		res = ParseGetPCI();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2://delay 5 second and check again
		count++;
		if(count >= 5000)
		{
			count = 0;
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void GetPciDisable(void)
{
	NbInfo.AppPro.bit.GetPCI = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

static void GetPCI(void)
{
	char buf[] = "AT+ECSTATUS\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetPCI(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0;
	char *temp;
	char num[3] = {0};
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))//response ok
	{
		temp = strstr((char *)ResDataBuf, "PCI:");
		if(temp != NULL)
		{
			for(i = 0; i < strlen(temp); i++)
			{
				if(temp[i + 4] == ',')
				{
					break;
				}
			}
			memcpy(num, &temp[4], i);//get PCI
			if(i == 1)
			{
				NbInfo.PCI = num[0] - '0';
			}
			else if(i == 2)
			{
				NbInfo.PCI = (num[0] - '0') * 10 + (num[1] - '0');
			}
			else if(i == 3)
			{
				NbInfo.PCI = (num[0] - '0') * 100 + (num[1] - '0') * 10 + (num[2] - '0');
			}
			NbInfo.InfoFlags.bit.PCI_Get = 1;
		}
		ResType = AT_RES_OK;
		GetPciDisable();
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void GetAttchEnable(void)
{
	NbInfo.AppPro.bit.GetAttach = 1;
}

static void GetAttachFunc(void)
{
	if(NbInfo.AppPro.bit.GetAttach)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_ATTACH))
		{
			RunGetAttach();
		}
	}
}

static void RunGetAttach(void)
{
	static uint8_t step = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0://Send AT command
		GetAttach();
		NbInfo.RunInStep = RUN_IN_GET_ATTACH;
		step++;
		break;
	case 1://Parse AT command
		res = ParseGetAttach();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 0;
			NbInfo.NetStatus = 0;
			GetAttachDisable();
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 0;
			NbInfo.NetStatus = 0;
			GetAttachDisable();
		}
		break;
	case 2:
		
		break;
	default:
			
		break;
	}
}

static void GetAttachDisable(void)
{
	NbInfo.AppPro.bit.GetAttach = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}


static void GetAttach(void)
{
	char buf[] = "AT+CGATT?\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetAttach(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	AT_ResType ParseRes = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0;
	
	count++;
	ResBuffAdd();
	ParseRes = NbParse();
	if(ParseRes == AT_RES_OK)//response ok
	{
		for(i = 0; i < ResDataIndex; i++)
		{
			if((ResDataBuf[i] >= '0') && (ResDataBuf[i] <= '1'))
			{
				NbInfo.NetStatus = ResDataBuf[i] - '0';
				break;
			}
		}
		if(NbInfo.NetStatus)
		{
			NbInfo.NoAttachTime = 0;
			if(!NbInfo.InfoFlags.bit.SocketCreate)
			{
				CreateSocketEnable();
			}
		}
		else
		{
			NbInfo.NoAttachTime++;
		}
		ResType = AT_RES_OK;
		GetAttachDisable();
	}
	else if(ParseRes == AT_RES_ERROR)//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void GetRssiEnable(void)
{
	NbInfo.AppPro.bit.GetRSSI = 1;
}

static void GetRssiFunc(void)
{
	if(NbInfo.AppPro.bit.GetRSSI)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_RSSI))
		{
			RunGetRSSI();
		}
	}
}

static void RunGetRSSI(void)
{
	static uint8_t step = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0:
		GetRSSI();
		NbInfo.RunInStep = RUN_IN_GET_RSSI;
		step++;
		break;
	case 1:
		res = ParseGetRSSI();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 0;
			GetRssiDisable();
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 0;
			GetRssiDisable();
		}
		break;
	case 2:
		
		break;
	default:
			
		break;
	}
}

static void GetRssiDisable(void)
{
	NbInfo.AppPro.bit.GetRSSI = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}


static void GetRSSI(void)
{
	char buf[] = "AT+CSQ\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetRSSI(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	AT_ResType ParseRes = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0, j = 0;
	
	count++;
	ResBuffAdd();
	ParseRes = NbParse();
	if(ParseRes == AT_RES_OK)//response ok
	{
		for(i = 0; i < ResDataIndex; i++)
		{
			if(ResDataBuf[i] >= '0' || ResDataBuf[i] <= '9')
			{
				break;
			}
		}
		//i++;
		for(j = i; j < ResDataIndex; j++)
		{
			if(ResDataBuf[j] == ',')
			{
				break;
			}
		}
		if((j - i) == 1)
		{
			NbInfo.RSSI = ResDataBuf[i] - '0';
		}
		else if((j - i) == 2)
		{
			NbInfo.RSSI = (ResDataBuf[i] - '0') * 10 + (ResDataBuf[i + 1] - '0');
		}
		if((NbInfo.NetStatus) && (!NbInfo.InfoFlags.bit.PCI_Get))
		{
			GetPciEnable();
		}
		if((NbInfo.NetStatus) && (!NbInfo.InfoFlags.bit.SelfIpGet))
		{
			GetIpEnable();
		}
		ResType = AT_RES_OK;
		GetRssiDisable();
	}
	else if(ParseRes == AT_RES_ERROR)//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void CreateSocketEnable(void)
{
	NbInfo.AppPro.bit.CreateSocket = 1;
}

static void CreateSocketFunc(void)
{
	if(NbInfo.AppPro.bit.CreateSocket)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_CREATE_SOCKET))
		{
			RunCreateSocket();
		}
	}
}

static void ConnectServer(void)
{
	char buf[] = "AT+SKTCONNECT=0,\"10.3.226.194\",9004\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseConnectServer(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	static uint16_t count = 0;
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))//response ok
	{
		NbInfo.InfoFlags.bit.SocketCreate = 1;
		ResType = AT_RES_OK;
		CreateSocketDisable();
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

static void RunCreateSocket(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0:
		CreateSocket();
		NbInfo.RunInStep = RUN_IN_CREATE_SOCKET;
		step++;
		break;
	case 1:
		res = ParseCreateSocket();
		if(res == AT_RES_OK)
		{
			step = 2;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 4;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 4;
		}
		break;
	case 2:
		ConnectServer();
		step++;
	break;
	case 3:
		res = ParseConnectServer();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 4;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 4;
		}
	break;
	case 4:
		count++;
		if(count >= (5 * 1000))
		{
			count = 0;
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void CreateSocketDisable(void)
{
	NbInfo.AppPro.bit.CreateSocket = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}


static void CreateSocket(void)
{
	char buf[] = "AT+SKTCREATE=1,2,17\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseCreateSocket(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0;
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))//response ok
	{
		for(i = 0; i < ResDataIndex; i++)
		{
			if((ResDataBuf[i] >= '0') && (ResDataBuf[i] <= '9'))
			{
				NbInfo.Socket = ResDataBuf[i];
				break;
			}
		}
		NbInfo.InfoFlags.bit.SocketCreate = 1;
		ResType = AT_RES_OK;
		
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void SendDataEnable(void)
{
	NbInfo.AppPro.bit.SendData = 1;
}

static void SendDataFunc(void)
{
	if(NbInfo.AppPro.bit.SendData)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_SEND_DATA))
		{
			RunSendData();
		}
	}
}

static void RunSendData(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	static uint8_t rty = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0:
		SenData();
		NbInfo.RunInStep = RUN_IN_SEND_DATA;
		step++;
		break;
	case 1:
		res = ParseSendData();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2:
		count++;
		if(count >= (5 * 1000))
		{
			count = 0;
			rty++;
			if(rty >= 3)
			{
				rty = 0;
				SendDataDisable();
				NbResetEnable();
			}
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void SendDataDisable(void)
{
	NbInfo.AppPro.bit.SendData = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

static void SenData(void)
{
#if 1
	//char buf[] = "AT+NSOCR=DGRAM,17,10001,1\r\n";
	SnedBufBuild();
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)sendBuf, SendBufIndex);
#else
	char buf[] = "AT+NSOST=1,124.221.84.132,10001,20,0102030405060708091011121314151617181920\r\n";
	
	buf[9] = NbInfo.Socket;
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, sizeof(buf));
#endif
}

static AT_ResType ParseSendData(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	AT_ResType ParseRes = AT_RES_CHECKING;
	static uint16_t count = 0;
	
	count++;
	ResBuffAdd();
	ParseRes = NbParse();
	if(ParseRes == AT_RES_OK)//response ok
	{
		//NbInfo.InfoFlags.bit.CheckResponse = 1;
		ResType = AT_RES_OK;
		SendDataDisable();
	}
	else if(ParseRes == AT_RES_ERROR)//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void ReadDataEnable(void)
{
	NbInfo.AppPro.bit.RevData = 1;
}

static void ReadDataFunc(void)
{
	if(NbInfo.AppPro.bit.RevData)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_REV_DATA))
		{
			RunReadData();
		}
	}
}


static void RunReadData(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	static uint8_t rty = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0:
		ReadData();
		NbInfo.RunInStep = RUN_IN_REV_DATA;
		step++;
		break;
	case 1:
		res = ParseReadData();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2:
		count++;
		if(count >= (5 * 1000))
		{
			count = 0;
			rty++;
			if(rty >= 3) 
			{
				rty = 0;
				ReadDataDisable();
				NbResetEnable();
			}
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void ReadDataDisable(void)
{
	NbInfo.AppPro.bit.RevData = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

static void ReadData(void)
{
	char buf[] = "AT+NSORF=0,100\r\n";
	
	buf[9] = NbInfo.Socket;
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, sizeof(buf));
}

static void RevDataRead(void)
{
	uint16_t i = 0, j = 0;
	char *readPoint = NULL;
	uint16_t LenTemp = 0;
	
	
	readPoint = strstr((char *)ResDataBuf, "+SKTRECV:");
	LenTemp = strlen(readPoint);
	for(i = 0; i < LenTemp; i++)
	{
		if(readPoint[i] == '\"')
		{
			break;
		}
	}
	i++;
	for(j = i; j < LenTemp; j++)
	{
		if(readPoint[j] == '\"')
		{
			break;
		}
	}
	RevBufIndex = (j - i) / 2;
	ReadData2Byte((char *)(&readPoint[i]), RevBufIndex);
}

static AT_ResType ParseReadData(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	AT_ResType ParseRes = AT_RES_CHECKING;
	static uint16_t count = 0;
	uint8_t i = 0, j = 0;
	static uint8_t Num = 0;
	
	count++;
	ResBuffAdd();
	ParseRes = NbParse();
	if(ParseRes == AT_RES_OK)//response ok
	{
		for(i = 0; i < ResDataIndex; i++)
		{
			if(ResDataBuf[i] == ',')
			{
				Num++;
				if(Num == 4)
				{
					Num = 0;
					break;
				}
			}
		}
		i++;
		for(j = i; j < ResDataIndex; j++)
		{
			if(ResDataBuf[j] == ',')
			{
				break;
			}
		}
		RevBufIndex = (j - i) / 2;
		ReadData2Byte((char *)(&ResDataBuf[i]), RevBufIndex);
		//need to add byte parse
		ResType = AT_RES_OK;
		ReadDataDisable();
	}
	else if(ParseRes == AT_RES_ERROR)//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	//if(count >= AT_COM_RES_TIMEOUT)
	if(count >= (10 * 1000))//10s timeout
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}

void GetIpEnable(void)
{
	NbInfo.AppPro.bit.GetIP = 1;
}

static void GetIpFunc(void)
{
	if(NbInfo.AppPro.bit.GetIP)
	{
		if((NbInfo.RunInStep == RUN_IN_NOTHING) || (NbInfo.RunInStep == RUN_IN_GET_IP))
		{
			RunGetIp();
		}
	}
}

static void RunGetIp(void)
{
	static uint8_t step = 0;
	static uint16_t count = 0;
	AT_ResType res;
	
	switch(step)
	{
	case 0:
		GetIp();
		NbInfo.RunInStep = RUN_IN_GET_IP;
		step++;
		break;
	case 1:
		res = ParseGetIp();
		if(res == AT_RES_OK)
		{
			step = 0;
		}
		else if(res == AT_RES_ERROR)
		{
			step = 2;
		}
		else if(res == AT_RES_TIMEOUT)
		{
			step = 2;
		}
		break;
	case 2:
		count++;
		if(count >= (5 * 1000))
		{
			count = 0;
			step = 0;
		}
		break;
	default:
			
		break;
	}
}

static void GetIpDisable(void)
{
	NbInfo.AppPro.bit.GetIP = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}


static void GetIp(void)
{
	char buf[] = "AT+CGPADDR=0\r\n";
	
	ClearRevBuf();
	NbUartSendBuf((uint8_t *)buf, strlen(buf));
}

static AT_ResType ParseGetIp(void)
{
	AT_ResType ResType = AT_RES_CHECKING;
	static uint16_t count = 0;
	
	count++;
	ResBuffAdd();
	if(strstr((char *)ResDataBuf, "OK\r\n"))//response ok
	{
		NbInfo.InfoFlags.bit.SelfIpGet = 1;
		ResType = AT_RES_OK;
		GetIpDisable();
	}
	else if(strstr((char *)ResDataBuf, "ERROR\r\n"))//response error
	{
		count = 0;
		ResType = AT_RES_ERROR;
	}
	if(count >= AT_COM_RES_TIMEOUT)
	{
		count = 0;
		ResType = AT_RES_TIMEOUT;
	}
	return ResType;
}


static void ClearRevBuf(void)
{
	memset(ResDataBuf, 0, sizeof(ResDataBuf));
	ResDataIndex = 0;
}

static void ResBuffAdd(void)
{
	uint8_t ch;
	
	if(NbCheck())
	{
		ch = NbGetData();
		ResDataBuf[ResDataIndex++] = ch;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				HostUartSendBuf(&ch, 1);
		}
	}
}

static void NbReset(void)
{
	static uint16_t count = 0;
	char *tempChar = NULL;
	
	count++;
	if(count == 1)
	{
		NbInfo.RunInStep = RUN_IN_MODULE_RESET;
		NbInfo.InfoFlags.bit.SocketCreate = 0;
		NB_RESET_HIGH;
	}
	else if(count == 50)
	{
		//NB_RESET_LOW;
		NB_RESET_HIGH;
	}
	else if(count == 7050)
	{
		//count = 0;
		//NB_RESET_HIGH;
		NB_RESET_LOW;
		//systemFlag.bit.NB_PowerReset = 1;
	}
	else if(count > 7050)
	{
		ResBuffAdd();
		tempChar = strstr((char *)ResDataBuf, "ECRDY\r\n");
		if(tempChar != NULL)
		{
			//if(strstr((char *)tempChar, "SIMST:1\r\n") || strstr((char *)tempChar, "SIMST:0\r\n"))//check reset is ok
			//{
				count = 0;
				NbResetDisable();
				if(!NbInfo.InfoFlags.bit.IMEI_Get)
				{
					GetImeiEnable();
				}
				if(!NbInfo.InfoFlags.bit.CCID_Get)
				{
					GetCcidEnable();
				}
				NbInfo.InfoFlags.bit.RefisterSuccess = 0;
				NbInfo.InfoFlags.bit.SendRegister = 0;
				NbInfo.NetStatus = 0;
				NbInfo.InfoFlags.bit.SocketCreate = 0;
				NbInfo.InfoFlags.bit.PCI_Get = 0;
				//CreateSocketEnable();
			//}
		}
		if(count >= 20350)//no response, reset again
		{
			count = 0;
		}
	}
}

static void NbResetDisable(void)
{
	NbInfo.AppPro.bit.ModuleReset = 0;
	NbInfo.RunInStep = RUN_IN_NOTHING;
}

void NbResetEnable(void)
{
	NbInfo.AppPro.bit.ModuleReset = 1;
}

static void NbResetFunc(void)
{
	if(NbInfo.AppPro.bit.ModuleReset)
	{
		if((NbInfo.RunInStep == RUN_IN_MODULE_RESET) || (NbInfo.RunInStep == RUN_IN_NOTHING))
		{
			NbReset();
		}
	}
}

void DebugNB(void)
{
	static uint8_t count = 0;
	uint8_t ch;
	static uint8_t buffer[250];
	static uint8_t index = 0;
	
	if(NbCheck())
	{
		count = 0;
		ch = NbGetData();
		buffer[index++] = ch;
	}
	else
	{
		count++;
		if(count >= 10)
		{
			if(index > 0)
			{
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
						HostUartSendBuf(buffer, index);
				}
				index = 0;
			}
			count = 0;
		}
	}
}
//check network status
static void CheckNet(void)
{
	static uint16_t count = 0;
	
	if(NbInfo.NoAttachTime >= 180)//180 * 10 = 0.5HOUR, 0.5 HOUR can't attach network
	{
		NbInfo.NoAttachTime = 0;
		NbResetEnable();
	}
	else 
	{
		count++;
		if(count >= 10000)//10 seconds to check network status
		{
			count = 0;
			GetAttchEnable();
			GetRssiEnable();
		}
	}
}

//asynoc data check
static void AsynCheck(void)
{
	uint8_t ch;
	static uint16_t count = 0;
	char *temp;
	
	if(NbCheck())//check "+NSOMI:" needto read data from server
	{
		NbInfo.RunInStep = RUN_IN_ASYN_CHECK;
		ch = NbGetData();
		ResDataBuf[ResDataIndex++] = ch;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				HostUartSendBuf(&ch, 1);
		}
		temp = strstr((char *)ResDataBuf, "+SKTRECV:");
		if(temp != NULL)
		{
			if(strstr(temp, "\r\n"))
			{
				//read enable
				//ReadDataEnable();//read data enable
				//ClearRevBuf();
				RevDataRead();
				NbInfo.RunInStep = RUN_IN_NOTHING;
			}
		}
	}
	else 
	{
		if(NbInfo.RunInStep == RUN_IN_ASYN_CHECK)//other response, no need action
		{
			count++;
			if(count >= 500)
			{
				count = 0;
				NbInfo.RunInStep = RUN_IN_NOTHING;
			}
		}
	}
}
//AT command response parse
static AT_ResType NbParse(void)
{
	AT_ResType res = AT_RES_CHECKING;
	char *str = NULL;
	
	str = strstr((char *)ResDataBuf, "+SKTRECV:");
	if(str != NULL)
	{
		if(strstr(str, "\r\n"))
		{
			//ReadDataEnable();//read data enable
			RevDataRead();
		}
	}
	if(strstr((char *)ResDataBuf, "OK\r\n"))
	{
		res = AT_RES_OK;
	}
	if(strstr((char *)ResDataBuf, "ERROR\r\n"))
	{
		res = AT_RES_ERROR;
	}
	return res;
}

//Nb module AT command application
static void NbApplication(void)
{
	uint8_t FuncID = 0;
	
	if(!NbInfo.AppPro.val || (NbInfo.RunInStep == RUN_IN_ASYN_CHECK))//check asynchronous data
	{
		AsynCheck();
	}
	else//run at command
	{
		for(FuncID = 0; FuncID < AT_FUNC_TABLE_COUNT; FuncID++)
		{
			if((1 << FuncID) & (NbInfo.AppPro.val))
			{
				AT_FuncTable[FuncID]();
			}
		}
	}
}

//resgister package control
//if no response, rty 3 times, if still no, wait 1 hour then register again
static void SendRegCheck(void)
{
	static uint32_t count = 0;
	static uint8_t RegTry = 0;
	
	//attach network, socket create, get CCID, get IMEI
	if((NbInfo.NetStatus == 1) && (NbInfo.InfoFlags.bit.SocketCreate) && (!NbInfo.InfoFlags.bit.SendRegister) && (NbInfo.InfoFlags.bit.CCID_Get) && (NbInfo.InfoFlags.bit.IMEI_Get) && (NbInfo.InfoFlags.bit.PCI_Get) && (NbInfo.RunInStep != RUN_IN_SEND_DATA))
	{
		//send register package
		NbInfo.InfoFlags.bit.SendRegister = 1;
		NbInfo.InfoFlags.bit.CheckRegRes = 1;
		RegTry++;
		NbInfo.Btype = SEND_REGISTER_BUF;
		SendDataEnable();
	}
	if(NbInfo.InfoFlags.bit.RegWaitHour)//wait 1 hour, then register again
	{
		count++;
		if(count >= (60 * 60 * 1000))//3600 seconds
		{
			count = 0; 
			NbInfo.InfoFlags.bit.RegWaitHour = 0;
			NbInfo.InfoFlags.bit.SendRegister = 0;//register again
		}
	}
	else//check whether need try.
	{
		if(NbInfo.InfoFlags.bit.CheckRegRes)//check register package response, try 3 times every minute, if no response, wait 1 hour then register again
		{
			count++;
			if(count >= (60 * 1000))//no response for 1 minute
			{
				count = 0;
				RegTry++;
				if(RegTry > 3)//send 3 times and no response
				{
					RegTry = 0;
					NbInfo.InfoFlags.bit.RegWaitHour = 1;
				}
				else//send try
				{
					if(NbInfo.RunInStep != RUN_IN_SEND_DATA)
					{
						NbInfo.Btype = SEND_REGISTER_BUF;
						SendDataEnable();
					}
				}
			}
		}
	}
}

static void SignalCheckLed(void)
{
#if 0
	if(NbInfo.Stype == SIGNAL_CLOSE)
	{
		if(NbInfo.Stime > StoreConf.ServerConf.CloseAlramTime)
		{
			DATA_LED_ALARM;
		}
		else if(NbInfo.Stime > StoreConf.ServerConf.AbnormalCloseTime)
		{
			//DATA_LED_ABNORMAL;
		}
		else
		{
			DATA_LED_NORMAL;
		}
	}
	else
	{
		if(NbInfo.Stime > StoreConf.ServerConf.OpenAlarmTime)
		{
			DATA_LED_ALARM;
		}
		else if(NbInfo.Stime > StoreConf.ServerConf.AbnormalOpenTime)
		{
			//DATA_LED_ABNORMAL;
		}
		else
		{
			DATA_LED_NORMAL;
		}
	}
#endif
}

//Interval Package and action package control
//rty 3 times if no response
static void SendDataCheck(void)
{
	static uint32_t count = 0;
	static uint8_t RegTry = 0;
	
	if(NbInfo.Chanel4_AlarmFlag)
	{
		Isl1208GetTime();
		StoreInfoAdd();
	}
	
	if((NbInfo.SendPackNum > 0) && NbInfo.NetStatus && NbInfo.InfoFlags.bit.RefisterSuccess && (NbInfo.RunInStep != RUN_IN_SEND_DATA) && (!NbInfo.InfoFlags.bit.CheckDataRes))//action send
	{
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("Send data first!\r\n");
		}
		RegTry = 0;
		count = 0; 
		RegTry++;
		//NbInfo.InfoFlags.bit.OpenCloseActSend = 0;
		NbInfo.InfoFlags.bit.CheckDataRes = 1;
		systemFlag.bit.SendSuccess = 0;
		SignalCheckLed();
		NbInfo.Btype = SEND_DATA_BUF;
		SendDataEnable();
	}
	if(systemFlag.bit.SendSuccess && (BackUpInfo.backupNum > 0) && (NbInfo.SendPackNum == 0) && NbInfo.NetStatus && NbInfo.InfoFlags.bit.RefisterSuccess && (NbInfo.RunInStep != RUN_IN_SEND_DATA) && (!NbInfo.InfoFlags.bit.CheckDataRes))
	{
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("Send backup data first!\r\n");
		}
		RegTry = 0;
		count = 0; 
		RegTry++;
		//NbInfo.InfoFlags.bit.OpenCloseActSend = 0;
		NbInfo.InfoFlags.bit.CheckDataRes = 1;
		systemFlag.bit.SendSuccess = 0;
		//SignalCheckLed();
		NbInfo.Btype = SEND_DATA_BUF;
		systemFlag.bit.BackUpSend = 1;
		SendDataEnable();
	}
#if 0
	if(NbInfo.InfoFlags.bit.Chanel4_AlarmSend && NbInfo.NetStatus && NbInfo.InfoFlags.bit.RefisterSuccess && (NbInfo.RunInStep != RUN_IN_SEND_DATA) && (!NbInfo.InfoFlags.bit.CheckDataRes))//channel4 alarm send
	{
		TX_ENABLE;
		printf("Channel 4 alarm send\r\n");
		RegTry++;
		NbInfo.RtyPack = 0;
		NbInfo.InfoFlags.bit.Chanel4_AlarmSend = 0;
		NbInfo.InfoFlags.bit.CheckDataRes = 1;
		NbInfo.Btype = SEND_DATA_BUF;
		SendDataEnable();
	}
	if(NbInfo.InfoFlags.bit.TempAlarmSend && NbInfo.NetStatus && NbInfo.InfoFlags.bit.RefisterSuccess && (NbInfo.RunInStep != RUN_IN_SEND_DATA) && (!NbInfo.InfoFlags.bit.CheckDataRes))//tempture alarm send
	{
		TX_ENABLE;
		printf("Temoture alarm send\r\n");
		RegTry++;
		NbInfo.RtyPack = 0;
		NbInfo.InfoFlags.bit.TempAlarmSend = 0;
		NbInfo.InfoFlags.bit.CheckDataRes = 1;
		NbInfo.Btype = SEND_DATA_BUF;
		SendDataEnable();
	}
	if(NbInfo.InfoFlags.bit.IntervalSend && NbInfo.NetStatus && NbInfo.InfoFlags.bit.RefisterSuccess && (NbInfo.RunInStep != RUN_IN_SEND_DATA) && (!NbInfo.InfoFlags.bit.CheckDataRes))//interval send
	{
		TX_ENABLE;
		printf("Interval send\r\n");
		RegTry++;
		NbInfo.RtyPack = 0;
		NbInfo.InfoFlags.bit.IntervalSend = 0;
		NbInfo.InfoFlags.bit.CheckDataRes = 1;
		NbInfo.Btype = SEND_DATA_BUF;
		SendDataEnable();
	}
#endif
	
	
	if(NbInfo.InfoFlags.bit.CheckDataRes)
	{
		count++;
		if(count >= (60 * 1000))//no response for 1 minute
		{
			count = 0;
			RegTry++;
			if(RegTry > 3)//send 3 times and no response
			{
				if(NbInfo.AlarmFlags.val)
				{
					NbInfo.AlarmSendOut = 1;
				}
				RegTry = 0;
				NbInfo.Stype = SIGNAL_NO;//clear open or close knife
				NbInfo.Stime = 0;
				NbInfo.InfoFlags.bit.CheckDataRes = 0;//no response for 3 times, do nothing
				NbInfo.Chanel4_AlarmFlag = 0x00;
				systemFlag.bit.SendSuccess = 0;
				if(!systemFlag.bit.BackUpSend)
				{
					BackUpAdd();
				}
				systemFlag.bit.BackUpSend = 0;
				if(NbInfo.SendPackNum > 0)
				{
					NbInfo.SendPackNum--;
					StoreInfo.NeedSendNum = NbInfo.SendPackNum;
					StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
					WriteStoreInfo();
				}
			}
			else//rty 3 times
			{
				if(NbInfo.RunInStep != RUN_IN_SEND_DATA)
				{
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("Send package rty\r\n");
					}
					NbInfo.Btype = SEND_DATA_BUF;
					SendDataEnable();
				}
			}
		}
	}
}
//configure package response
static void ConfResCheck(void)
{
	if(NbInfo.InfoFlags.bit.ConfResSend && (NbInfo.RunInStep != RUN_IN_SEND_DATA))
	{
		NbInfo.InfoFlags.bit.ConfResSend = 0;
 		NbInfo.Btype = SEND_REVEIVE_BUF;
		SendDataEnable();
	} 
}

void NB_Control(void)
{
	//DebugNB();
#if 1
	if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
	{
		CheckNet();
		SendRegCheck();
		SendDataCheck();
		ConfResCheck();
		NbApplication();
	}
	else//modbus communicate, need to get IMEI
	{
		if(!NbInfo.InfoFlags.bit.IMEI_Get)
		{
			NbApplication();
		}
	}
#endif
}

