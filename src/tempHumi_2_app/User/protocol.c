#include "protocol.h"
#include "hw_uart.h"
static uint8_t signalBuf[7] = {0};
static void ClearData(void);
static void BuildSendProPak(ComEnum command, uint8_t *buf, uint8_t len);
static void GetWlStatusEnable(void);
static void GetWlStatusDisable(void);
static void RunGetWlStatus(void);
static void GetWlStatusFun(void);
static void GetTempIdEnable(void);
static void GetTempIdDisable(void);
static void RunGetTempId(void);
static void GetTempIdFun(void);
static void GetHumiIdEnable(void);
static void GetHumiIdDisable(void);
static void RunGetHumiId(void);
static void GetHumiIdFun(void);
static void GetTempValEnable(void);
static void GetTempValDisable(void);
static void RunGetTempVal(void);
static void GetTempValFun(void);
static void GetHumiValEnable(void);
static void GetHumipValDisable(void);
static void RunGetHumiVal(void);
static void GetHumiValFun(void);
static void SetSelfTempHumiEnable(void);
static void SetSelfTempHumiDisable(void);
static void RunSetSelfTempHumi(void);
static void SetSelfTempHumiFun(void);
static void SendSignalEnable(void);
static void SendSignalDisable(void);
static void RunSendSignal(void);
static void SendSignalFun(void);
static void SetAbnormalEnable(void);
static void SetAbnormalDisable(void);
static void RunSetAbnormal(void);
static void SetAbnormalFun(void);
static void SendCommuncationStatusEnable(void);
static void SendCommuncationStatusDisable(void);
static void RunSendCommuncationStatus(void);
static void SendCommuncationStatusFun(void);
static void SendMainBoardInfoDisable(void);
static void RunSendMainBoardInfo(void);
static void SendMainBoardInfoFun(void);
ProtocolStr ProStr;
ProFunStr ProFunctionFlag;

Profunc funTable[] = {
	GetWlStatusFun,
	GetTempIdFun,
	GetHumiIdFun,
	GetTempValFun,
	GetHumiValFun,
	SetSelfTempHumiFun,
	SendSignalFun,
	SetAbnormalFun,
	SendCommuncationStatusFun,
	SendMainBoardInfoFun,
};

static uint8_t GetParseProtocolStatus(void)
{
		static uint16_t count = 0;
	
		count++;
		if(ProStr.ParseDone)
		{
				count = 0;
				return 1;
		}
		if(count >= 300)
		{
				count = 0;
				return 2;
		}
		return 0;
}

static void BuildSendProPak(ComEnum command, uint8_t *buf, uint8_t len)
{
		uint8_t data[200] = {0};
		uint16_t crc = 0;
		uint8_t i = 0;
		
		data[i++] = 0x5A;
		data[i++] = len + 3;
		data[i++] = command;
		data[i++] = 0x01;
		if(len > 0)
		{
				memcpy(&data[i], buf, len);
		}
		i += len;
		crc = Modbus_Crc_Compute(data, i);
		data[i++] = crc >> 8;
		data[i++] = crc;
		data[i++] = 0x5B;
		CommunUartSendBuf(data, i);
}

void SendMainBoardInfoEnable(void)
{
		ProFunctionFlag.enableFlag.bit.SendMainBoardInfo = 1;
}

static void SendMainBoardInfoDisable(void)
{
		ProFunctionFlag.enableFlag.bit.SendMainBoardInfo = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunSendMainBoardInfo(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t buf[32] = {0};

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_SEND_MAIN_BOARD_INFO;
				buf[0] = StoreConf.SlaveAddr;
				buf[1] = StoreConf.BaudRateModbus >> 24;
				buf[2] = StoreConf.BaudRateModbus >> 16;
				buf[3] = StoreConf.BaudRateModbus >> 8;
				buf[4] = StoreConf.BaudRateModbus >> 0;
				memcpy(&buf[5], &NbInfo.IMEI_BYTE[1], 15);
				memcpy(&buf[20], Version, 6);
				buf[26] = NbInfo.uTime >> 24;
				buf[27] = NbInfo.uTime >> 16;
				buf[28] = NbInfo.uTime >> 8;
				buf[29] = NbInfo.uTime >> 0;
				buf[30] = NbInfo.NetLedStatus;
				buf[31] = NbInfo.DataLedStatus;
				BuildSendProPak(COMMAND_SEND_MAIN_BOARD_INFO, buf, sizeof(buf));				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						//ProFunctionFlag.getFlag.bit.CommunicationOk = 1;
						SendMainBoardInfoDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						SendMainBoardInfoDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void SendMainBoardInfoFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_SEND_MAIN_BOARD_INFO)
		{
				if(ProFunctionFlag.enableFlag.bit.SendMainBoardInfo)
				{
						RunSendMainBoardInfo();
				}
		}	
}

static void SendCommuncationStatusEnable(void)
{
		ProFunctionFlag.enableFlag.bit.SendCommunicateStatus = 1;
}

static void SendCommuncationStatusDisable(void)
{
		ProFunctionFlag.enableFlag.bit.SendCommunicateStatus = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunSendCommuncationStatus(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t buf[1] = {0x01};

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_SEND_COMMUNICATION_STATUS;
				BuildSendProPak(COMMAND_SEND_COMMUNICATION_STATUS, buf, 1);				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						ProFunctionFlag.getFlag.bit.CommunicationOk = 1;
						SendCommuncationStatusDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						SendCommuncationStatusDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void SendCommuncationStatusFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_SEND_COMMUNICATION_STATUS)
		{
				if(ProFunctionFlag.enableFlag.bit.SendCommunicateStatus)
				{
						RunSendCommuncationStatus();
				}
		}	
}

static void SendSignalEnable(void)
{
		ProFunctionFlag.enableFlag.bit.SendSignalInfoEnable = 1;
}

static void SendSignalDisable(void)
{
		ProFunctionFlag.enableFlag.bit.SendSignalInfoEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunSendSignal(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		//uint8_t buf[7] = {0};

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_SEND_SIGNAL;
				BuildSendProPak(COMMAND_SEND_SIGNAL_INFO, signalBuf, 7);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						//ProFunctionFlag.getFlag.bit.WlIvild = ProStr.buf[0];
						SendSignalDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						SendSignalDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void SendSignalFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_SEND_SIGNAL)
		{
				if(ProFunctionFlag.enableFlag.bit.SendSignalInfoEnable)
				{
						RunSendSignal();
				}
		}	
}

static void SetSelfTempHumiEnable(void)
{
		ProFunctionFlag.enableFlag.bit.SeSelfTempHumiEnable = 1;
}

static void SetSelfTempHumiDisable(void)
{
		ProFunctionFlag.enableFlag.bit.SeSelfTempHumiEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunSetSelfTempHumi(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0, i = 0;
		uint8_t buf[53] = {0};

		switch(step)
		{
			case 0:
				buf[i++] = 0xFF;
				buf[i++] = TempHumiData.selfTemp[0] >> 8;
				buf[i++] = TempHumiData.selfTemp[0];
				buf[i++] = TempHumiData.selfTempUnix[0] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[0] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[0] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[0];
				buf[i++] = TempHumiData.selfTemp[1] >> 8;
				buf[i++] = TempHumiData.selfTemp[1];
				buf[i++] = TempHumiData.selfTempUnix[1] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[1] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[1] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[1];
				buf[i++] = TempHumiData.selfTemp[2] >> 8;
				buf[i++] = TempHumiData.selfTemp[2];
				buf[i++] = TempHumiData.selfTempUnix[2] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[2] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[2] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[2];
				buf[i++] = TempHumiData.selfTemp[3] >> 8;
				buf[i++] = TempHumiData.selfTemp[3];
				buf[i++] = TempHumiData.selfTempUnix[3] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[3] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[3] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[3];
				buf[i++] = TempHumiData.selfTemp[4] >> 8;
				buf[i++] = TempHumiData.selfTemp[4];
				buf[i++] = TempHumiData.selfTempUnix[4] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[4] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[4] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[4];
				buf[i++] = TempHumiData.selfTemp[5] >> 8;
				buf[i++] = TempHumiData.selfTemp[5];
				buf[i++] = TempHumiData.selfTempUnix[5] >> 24;
				buf[i++] = TempHumiData.selfTempUnix[5] >> 16;
				buf[i++] = TempHumiData.selfTempUnix[5] >> 8;
				buf[i++] = TempHumiData.selfTempUnix[5];
				buf[i++] = TempHumiData.selfHumi >> 8;
				buf[i++] = TempHumiData.selfHumi;
				buf[i++] = TempHumiData.selfHumiUnix >> 24;
				buf[i++] = TempHumiData.selfHumiUnix >> 16;
				buf[i++] = TempHumiData.selfHumiUnix >> 8;
				buf[i++] = TempHumiData.selfHumiUnix;
				buf[i++] = ((uint16_t)(NbInfo.InterTemp - 100) * 10) >> 8;
				buf[i++] = ((uint16_t)(NbInfo.InterTemp - 100) * 10);
				buf[i++] = NbInfo.Cj2301Temp >> 8;
				buf[i++] = NbInfo.Cj2301Temp;
				buf[i++] = NbInfo.Cj2301Humi >> 8;
				buf[i++] = NbInfo.Cj2301Humi;
				buf[i++] = NbInfo.uTime >> 24;
				buf[i++] = NbInfo.uTime >> 16;
				buf[i++] = NbInfo.uTime >> 8;
				buf[i++] = NbInfo.uTime;
				ClearData();
				ProFunctionFlag.runStep = RUN_SELF_TEMP_HUMI;
				BuildSendProPak(COMMAND_SET_SELF_TEMP_HUMI, buf, 53);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						//ProFunctionFlag.getFlag.bit.WlIvild = ProStr.buf[0];
						SetSelfTempHumiDisable();
						step = 0;
				}
				else if(ret == 2)
				{	
						SetSelfTempHumiDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void SetSelfTempHumiFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_SELF_TEMP_HUMI)
		{
				if(ProFunctionFlag.enableFlag.bit.SeSelfTempHumiEnable)
				{
						RunSetSelfTempHumi();
				}
		}	
}

static void GetHumiValEnable(void)
{
		ProFunctionFlag.enableFlag.bit.GetHumiValEnable = 1;
}

static void GetHumipValDisable(void)
{
		ProFunctionFlag.enableFlag.bit.GetHumiValEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunGetHumiVal(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0, i = 0, j = 0;

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_GET_HUMI_VAL;
				BuildSendProPak(COMMAND_GET_HUMI_VAL, NULL, 0);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						for(i = 0; i < 15; i++)
						{
								TempHumiData.humiVal[i] = (uint16_t)(ProStr.buf[j] << 8) + ProStr.buf[j + 1];
								j += 2;
						}
						GetHumipValDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						GetHumipValDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void GetHumiValFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_GET_HUMI_VAL)
		{
				if(ProFunctionFlag.enableFlag.bit.GetHumiValEnable)
				{
						RunGetHumiVal();
				}
		}	
}

static void GetTempValEnable(void)
{
		ProFunctionFlag.enableFlag.bit.GetTempValEnable = 1;
}

static void GetTempValDisable(void)
{
		ProFunctionFlag.enableFlag.bit.GetTempValEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunGetTempVal(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0, i = 0, j = 0;

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_GET_TEMP_VAL;
				BuildSendProPak(COMMADN_GET_TEMP_VAL, NULL, 0);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						for(i = 0; i < 90; i++)
						{
								TempHumiData.tempVal[i] = (uint16_t)(ProStr.buf[j] << 8) + ProStr.buf[j + 1];
								j += 2;
						}
						GetTempValDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						GetTempValDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void GetTempValFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_GET_TEMP_VAL)
		{
				if(ProFunctionFlag.enableFlag.bit.GetTempValEnable)
				{
						RunGetTempVal();
				}
		}	
}

static void GetHumiIdEnable(void)
{
		ProFunctionFlag.enableFlag.bit.GetHumiIdEnable = 1;
}

static void GetHumiIdDisable(void)
{
		ProFunctionFlag.enableFlag.bit.GetHumiIdEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunGetHumiId(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0, i = 0, j = 1;

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_GET_HUMI_ID;
				BuildSendProPak(COMMAND_GET_HUMI_ID, NULL, 0);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						if(ProStr.buf[0])
						{
								ProFunctionFlag.getFlag.bit.HumiIDGet = 1;
								for(i = 0; i < 15; i++)
								{
										TempHumiData.humiID[i] = (uint16_t)(ProStr.buf[j] << 8) + ProStr.buf[j + 1];
										j += 2;
								}
						}
						GetHumiIdDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						GetHumiIdDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void GetHumiIdFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_GET_HUMI_ID)
		{
				if(ProFunctionFlag.enableFlag.bit.GetHumiIdEnable)
				{
						RunGetHumiId();
				}
		}	
}

static void GetTempIdEnable(void)
{
		ProFunctionFlag.enableFlag.bit.GetTempIdEnable = 1;
}

static void GetTempIdDisable(void)
{
		ProFunctionFlag.enableFlag.bit.GetTempIdEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunGetTempId(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0, i = 0, j = 1;

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_GET_TEMP_ID;
				BuildSendProPak(COMMAND_GET_TEMP_ID, NULL, 0);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						if(ProStr.buf[0])
						{
								ProFunctionFlag.getFlag.bit.TempIDGet = 1;
								for(i = 0; i < 90; i++)
								{
										TempHumiData.tempID[i] = (uint16_t)(ProStr.buf[j] << 8) + ProStr.buf[j + 1];
										j += 2;
								}
						}
						GetTempIdDisable();
						step = 0;
				}
				else if(ret == 2)
				{	
						GetTempIdDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void GetTempIdFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_GET_TEMP_ID)
		{
				if(ProFunctionFlag.enableFlag.bit.GetTempIdEnable)
				{
						RunGetTempId();
				}
		}	
}

static void GetWlStatusEnable(void)
{
		ProFunctionFlag.enableFlag.bit.GetWlEnable = 1;
}
static void GetWlStatusDisable(void)
{
		ProFunctionFlag.enableFlag.bit.GetWlEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunGetWlStatus(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_GET_WL_TEMP_STATUS;
				BuildSendProPak(COMMAND_GET_WL_STATUS, NULL, 0);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						ProFunctionFlag.getFlag.bit.WlIvild = ProStr.buf[0];
						if(!ProFunctionFlag.getFlag.bit.CommunicationOk)
						{
								SendCommuncationStatusEnable();
						}
						GetWlStatusDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						GetWlStatusDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void GetWlStatusFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_GET_WL_TEMP_STATUS)
		{
				if(ProFunctionFlag.enableFlag.bit.GetWlEnable)
				{
						RunGetWlStatus();
				}
		}	
}

static void SetAbnormalEnable(void)
{
		ProFunctionFlag.enableFlag.bit.SetAbnormalEnable = 1;
}
static void SetAbnormalDisable(void)
{
		ProFunctionFlag.enableFlag.bit.SetAbnormalEnable = 0;
		ProFunctionFlag.runStep = RUN_NOTHING;
}

static void RunSetAbnormal(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t buf[6] = {0};

		switch(step)
		{
			case 0:
				ClearData();
				ProFunctionFlag.runStep = RUN_SET_ABNORMAL;
				buf[0] = NbInfo.ConfData.AbnormalTem;
				buf[1] = NbInfo.ConfData.AbnormalHumi;
				buf[2] = NbInfo.ConfData.AbnormalCloseTime >> 8;
				buf[3] = NbInfo.ConfData.AbnormalCloseTime;
				buf[4] = NbInfo.ConfData.AbnormalOpenTime >> 8;
				buf[5] = NbInfo.ConfData.AbnormalOpenTime;
				BuildSendProPak(COMMAND_SET_ABNORMAL_VAL, buf, 6);
				
				step++;
			break;
			case 1:
				ret = GetParseProtocolStatus();
				if(ret == 1)
				{
						//ProFunctionFlag.getFlag.bit.WlIvild = ProStr.buf[0];
						SetAbnormalDisable();
						step = 0;
				}
				else if(ret == 2)
				{
						SetAbnormalDisable();
						step = 0;
				}
			break;
			default:
				
			break;
		}
}

static void SetAbnormalFun(void)
{
		if(ProFunctionFlag.runStep == RUN_NOTHING || ProFunctionFlag.runStep == RUN_SET_ABNORMAL)
		{
				if(ProFunctionFlag.enableFlag.bit.SetAbnormalEnable)
				{
						RunSetAbnormal();
				}
		}	
}

static void ClearData(void)
{
		ProStr.step = PRO_HEAD;
		ProStr.ParseDone = 0;
}

void ParsePro(uint8_t ch)
{
		static uint8_t tempLen = 0;
		static uint8_t tempBuf[200] = {0};
		static uint8_t tempBufLen = 0;
		static uint16_t tempCrc = 0;
	
		switch(ProStr.step)
		{
			case PRO_HEAD:
				if(ch == PRO_HRAD_VAL)
				{
						ProStr.bufIndex = 0;
						ProStr.len = 0;
						tempLen = 0;
						ProStr.step = PRO_LEN;
						tempBufLen = 0;
						tempBuf[tempBufLen++] = ch;
				}
			break;
			case PRO_LEN:
				if(ch >= 3)
				{
						ProStr.len = ch;
						tempLen = ch - 3;
						ProStr.step = PRO_COMMAND;
						tempBuf[tempBufLen++] = ch;
				}
				else
				{
						ProStr.step = PRO_HEAD;
				}
			break;
			case PRO_COMMAND:
				ProStr.command = ch;
				ProStr.step = PRO_COM_VERSION;
				tempBuf[tempBufLen++] = ch;
			break;
			case PRO_COM_VERSION:
				if(ch == 0x01)
				{
						if(tempLen > 0)
						{
								ProStr.step = PRO_DATA;
						}
						else
						{
								ProStr.step = PRO_CRC_H;
						}
						tempBuf[tempBufLen++] = ch;
				}
				else
				{
						ProStr.step = PRO_HEAD;
				}
			break;
			case PRO_DATA:
				ProStr.buf[ProStr.bufIndex++] = ch;
				tempBuf[tempBufLen++] = ch;
				if((--tempLen) == 0)
				{
						ProStr.step = PRO_CRC_H;
				}
			break;
			case PRO_CRC_H:
				tempCrc = Modbus_Crc_Compute(tempBuf, tempBufLen);
				if(ch == (uint8_t)(tempCrc >> 8))
				{
					ProStr.step = PRO_CRC_L;
				}
				else
				{
					ProStr.step = PRO_HEAD;
				}
			break;
			case PRO_CRC_L:
				if(ch == (uint8_t)(tempCrc))
				{
					ProStr.step = PRO_TAIL;
				}
				else
				{
					ProStr.step = PRO_HEAD;
				}
			break;
			case PRO_TAIL:
				if(ch == PRO_TAIL_VAL)
				{
						ProStr.ParseDone = 1;
				}
				ProStr.step = PRO_HEAD;
			break;
			default:
				
			break;
		}
}

static void ProCheckRunFunc(void)
{
		static uint16_t count1 = 0, count2 = 0;

				
		count1++;
		count2++;
		if(count1 >= 1000)
		{
				count1 = 0;
				if(!ProFunctionFlag.getFlag.bit.WlIvild)
				{
						GetWlStatusEnable();
				}
				if(ProFunctionFlag.getFlag.bit.WlIvild)
				{
						GetTempIdEnable();
						GetHumiIdEnable();
				}
				if(ProFunctionFlag.getFlag.bit.TempIDGet)
				{
						GetTempValEnable();
				}
				if(ProFunctionFlag.getFlag.bit.HumiIDGet)
				{
						GetHumiValEnable();
				}
				SendMainBoardInfoEnable();
		}
		if(count2 >= 2000)
		{
				count2 = 0;
				//if(ProFunctionFlag.getFlag.bit.TempIDGet && ProFunctionFlag.getFlag.bit.HumiIDGet)
				//{
						SetSelfTempHumiEnable();
				//}
				if(NbInfo.SendDisplayPakNum > 0)
				{						
						if(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].SignalType != SIGNAL_NO)
						{
								if(StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].SignalType == SIGNAL_CLOSE)
								{
										signalBuf[0] = 0;
								}
								else
								{
										signalBuf[0] = 1;
								}
								//signalBuf[0] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].SignalType;
								signalBuf[1] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].SignalTime >> 8;
								signalBuf[2] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].SignalTime;
								signalBuf[3] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].UnixTime >> 24;
								signalBuf[4] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].UnixTime >> 16;
								signalBuf[5] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].UnixTime >> 8;
								signalBuf[6] = StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendDisplayPakNum].UnixTime;
								
								SendSignalEnable();
						}
						NbInfo.SendDisplayPakNum--;
				}
				SetAbnormalEnable();
		}
}

void ProtocolTask(void)
{
		uint8_t i = 0;

		if(!systemFlag.bit.BypassMode)
		{
				ProCheckRunFunc();
				for(i = 0; i < sizeof(funTable) / sizeof(Profunc); i++)
				{
						funTable[i]();
				}
		}
}

