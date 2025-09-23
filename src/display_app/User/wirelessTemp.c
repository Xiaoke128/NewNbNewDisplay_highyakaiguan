#include "wirelessTemp.h"
#include "modbus.h"
#include "hw_uart.h"
WlStr wlStrData = {0};

static void RunCheckConnect(void);
static void RunTempID(void);
static void RunTempVal(void);
static void RunHumiID(void);
static void RunHumiVal(void);

pFunction wlFunTable[] = {
	RunCheckConnect,
	RunTempID,
	RunTempVal,
	RunHumiID,
	RunHumiVal,
};

static uint8_t ParseModbus(void)
{
		static uint16_t count = 0;
	
		count++;
		while(NbCheck())
		{
				modbusMasterParse(NbGetData());
		}
		if(modbus.ParseDone)
		{
				return 1;
		}
		if(count >= 300)
		{
				count = 0;
				return 2;
		}
		return 0;
}

static void CheckConnectEnable(void)
{
		wlStrData.flag.bit.getConnect = 1;
}

static void CheckConnectDisable(void)
{
		wlStrData.flag.bit.getConnect = 0;
		wlStrData.runStep = WL_RUNNING_NOTHING;
}

static void SendCheckConnect(void)
{
		uint8_t data[8] = {0};
		uint16_t crc = 0;
				
		data[0] = 0x01;
		data[1] = 0x03;
		data[2] = 0x00;
		data[3] = 0x00;
		data[4] = 0x00;
		data[5] = 0x01;
		crc = Modbus_Crc_Compute(data, 6);
		data[6] = crc;
		data[7] = crc >> 8;
		NbUartSendBuf(data, 8);
}

static void CheckConnectFun(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
	
		switch(step)
		{
			case 0://send read command
				SendCheckConnect();
				modbusValInit();
				wlStrData.runStep = WL_RUNNING_CHECK_CONNECT;
				step++;
			break;
			case 1://parse data
				ret = ParseModbus();
				if(ret == 1)
				{
						wlStrData.sysFlag.bit.Connected = 1;
						step = 0;
						CheckConnectDisable();
				}
				else if(ret == 2)
				{
						step = 0;
						CheckConnectDisable();
				}
			break;
			default:
				
			break;
		}
}

static void RunCheckConnect(void)
{
		if(wlStrData.runStep == WL_RUNNING_NOTHING || wlStrData.runStep == WL_RUNNING_CHECK_CONNECT)
		{
				if(wlStrData.flag.bit.getConnect)
				{
						CheckConnectFun();
				}
		}
}

static void TempIDEnable(void)
{
		wlStrData.flag.bit.getTempID = 1;
}

static void TempIDDisable(void)
{
		wlStrData.flag.bit.getTempID = 0;
		wlStrData.runStep = WL_RUNNING_NOTHING;
}

static void SendTempID(void)
{
		uint8_t data[8] = {0};
		uint16_t crc = 0;
				
		data[0] = 0x01;
		data[1] = 0x03;
		data[2] = 0x00;
		data[3] = 0xF4;
		data[4] = 0x00;
		data[5] = 0x5A;
		crc = Modbus_Crc_Compute(data, 6);
		data[6] = crc;
		data[7] = crc >> 8;
		NbUartSendBuf(data, 8);
}

static void CheckTempIDFunc(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t i = 0, j = 1;
	
		switch(step)
		{
			case 0://send read command
				SendTempID();
				modbusValInit();
				wlStrData.runStep = WL_RUNNING_GET_TEMP_ID;
				step++;
			break;
			case 1://parse data
				ret = ParseModbus();
				if(ret == 1)
				{
						for(i = 0; i < 90; i++)
						{
								wlStrData.tempID_Val[i][0] = ((uint16_t)modbus.data[j] << 8) + (uint16_t)modbus.data[j + 1];
								j += 2;
						}
						wlStrData.sysFlag.bit.TempIDGet = 1;
						step = 0;
						TempIDDisable();
				}
				else if(ret == 2)
				{
						step = 0;
						TempIDDisable();
				}
			break;
			default:
				
			break;
		}
}

static void RunTempID(void)
{
		if(wlStrData.runStep == WL_RUNNING_NOTHING || wlStrData.runStep == WL_RUNNING_GET_TEMP_ID)
		{
				if(wlStrData.flag.bit.getTempID)
				{
						CheckTempIDFunc();
				}
		}
}

static void TempValEnable(void)
{
		wlStrData.flag.bit.getTempVal = 1;
}

static void TempValDisable(void)
{
		wlStrData.flag.bit.getTempVal = 0;
		wlStrData.runStep = WL_RUNNING_NOTHING;
}

static void SendTempVal(void)
{
		uint8_t data[8] = {0};
		uint16_t crc = 0;
				
		data[0] = 0x01;
		data[1] = 0x03;
		data[2] = 0x00;
		data[3] = 0x04;
		data[4] = 0x00;
		data[5] = 0x5A;
		crc = Modbus_Crc_Compute(data, 6);
		data[6] = crc;
		data[7] = crc >> 8;
		NbUartSendBuf(data, 8);
}

static void CheckTempValFunc(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t i = 0, j = 1;
	
		switch(step)
		{
			case 0://send read command
				SendTempVal();
				modbusValInit();
				wlStrData.runStep = WL_RUNNING_GET_TEMP_VAL;
				step++;
			break;
			case 1://parse data
				ret = ParseModbus();
				if(ret == 1)
				{
						for(i = 0; i < 90; i++)
						{
								wlStrData.tempID_Val[i][1] = ((uint16_t)modbus.data[j] << 8) + (uint16_t)modbus.data[j + 1];
								j += 2;
						}
						step = 0;
						TempValDisable();
				}
				else if(ret == 2)
				{
						step = 0;
						TempValDisable();
				}
			break;
			default:
				
			break;
		}
}

static void RunTempVal(void)
{
		if(wlStrData.runStep == WL_RUNNING_NOTHING || wlStrData.runStep == WL_RUNNING_GET_TEMP_VAL)
		{
				if(wlStrData.flag.bit.getTempVal)
				{
						CheckTempValFunc();
				}
		}
}

static void HumiIDEnable(void)
{
		wlStrData.flag.bit.getHumiID = 1;
}

static void HumiIDDisable(void)
{
		wlStrData.flag.bit.getHumiID = 0;
		wlStrData.runStep = WL_RUNNING_NOTHING;
}

static void SendHumiID(void)
{
		uint8_t data[8] = {0};
		uint16_t crc = 0;
				
		data[0] = 0x01;
		data[1] = 0x03;
		data[2] = 0x01;
		data[3] = 0x58;
		data[4] = 0x00;
		data[5] = 0x0F;
		crc = Modbus_Crc_Compute(data, 6);
		data[6] = crc;
		data[7] = crc >> 8;
		NbUartSendBuf(data, 8);
}

static void HumiIDFunc(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t i = 0, j = 1;
	
		switch(step)
		{
			case 0://send read command
				SendHumiID();
				modbusValInit();
				wlStrData.runStep = WL_RUNNING_GET_HUMI_ID;
				step++;
			break;
			case 1://parse data
				ret = ParseModbus();
				if(ret == 1)
				{
						for(i = 0; i < 15; i++)
						{
								wlStrData.humiID_Val[i][0] = ((uint16_t)modbus.data[j] << 8) + (uint16_t)modbus.data[j + 1];
								j += 2;
						}
						wlStrData.sysFlag.bit.HumiIDGet = 1;
						step = 0;
						HumiIDDisable();
				}
				else if(ret == 2)
				{
						step = 0;
						HumiIDDisable();
				}
			break;
			default:
				
			break;
		}
}

static void RunHumiID(void)
{
		if(wlStrData.runStep == WL_RUNNING_NOTHING || wlStrData.runStep == WL_RUNNING_GET_HUMI_ID)
		{
				if(wlStrData.flag.bit.getHumiID)
				{
						HumiIDFunc();
				}
		}
}

static void HumiValEnable(void)
{
		wlStrData.flag.bit.getHumiVal = 1;
}

static void HumiValDisable(void)
{
		wlStrData.flag.bit.getHumiVal = 0;
		wlStrData.runStep = WL_RUNNING_NOTHING;
}

static void SendHumiVal(void)
{
		uint8_t data[8] = {0};
		uint16_t crc = 0;
				
		data[0] = 0x01;
		data[1] = 0x03;
		data[2] = 0x02;
		data[3] = 0x67;
		data[4] = 0x00;
		data[5] = 0x0F;
		crc = Modbus_Crc_Compute(data, 6);
		data[6] = crc;
		data[7] = crc >> 8;
		NbUartSendBuf(data, 8);
}

static void HumiValFunc(void)
{
		static uint8_t step = 0;
		uint8_t ret = 0;
		uint8_t i = 0, j = 1;
	
		switch(step)
		{
			case 0://send read command
				SendHumiVal();
				modbusValInit();
				wlStrData.runStep = WL_RUNNING_GET_HUMI_VAL;
				step++;
			break;
			case 1://parse data
				ret = ParseModbus();
				if(ret == 1)
				{
						for(i = 0; i < 15; i++)
						{
								wlStrData.humiID_Val[i][1] = ((uint16_t)modbus.data[j] << 8) + (uint16_t)modbus.data[j + 1];
								j += 2;
						}
						step = 0;
						HumiValDisable();
				}
				else if(ret == 2)
				{
						step = 0;
						HumiValDisable();
				}
			break;
			default:
				
			break;
		}
}

static void RunHumiVal(void)
{
		if(wlStrData.runStep == WL_RUNNING_NOTHING || wlStrData.runStep == WL_RUNNING_GET_HUMI_VAL)
		{
				if(wlStrData.flag.bit.getHumiVal)
				{
						HumiValFunc();
				}
		}
}


static void RunFunc(void)
{
		uint8_t i = 0;
		
		for(i = 0; i < 5; i++)
		{
				wlFunTable[i]();
		}
}

static void EnbaleFunc(void)
{
		static uint16_t count1 = 0, count2 = 0, count3 = 0, count4 = 0;
	
		count1++;
		count2++;
		count3++;
		count4++;
		if(count1 >= 5000)
		{
				count1 = 0;
				if(!wlStrData.sysFlag.bit.Connected)
				{
						CheckConnectEnable();
				}
		}
		if(count2 >= 2000)
		{
				count2 = 0;
				if(!wlStrData.sysFlag.bit.TempIDGet && wlStrData.sysFlag.bit.Connected)
				{
						TempIDEnable();
				}
		}
		if(count3 >= 2000)
		{
				count3 = 0;
				if(!wlStrData.sysFlag.bit.HumiIDGet && wlStrData.sysFlag.bit.Connected)
				{
						HumiIDEnable();
				}
		}
		if(count4 >= 1000)
		{
				count4 = 0;
				if(wlStrData.sysFlag.bit.Connected)
				{
						if(wlStrData.sysFlag.bit.TempIDGet)
						{
								TempValEnable();
						}
						if(wlStrData.sysFlag.bit.HumiIDGet)
						{
								HumiValEnable();
						}
				}
		}
}

void WlVarInit(void)
{
		memset(&wlStrData, 0, sizeof(wlStrData));
		memset(wlStrData.humiID_Val, 0xFF, sizeof(wlStrData.humiID_Val));
		memset(wlStrData.tempID_Val, 0xFF, sizeof(wlStrData.tempID_Val));
		CheckConnectEnable();
}

void GetWirelessTempTask(void)
{
		EnbaleFunc();
		RunFunc();
}

