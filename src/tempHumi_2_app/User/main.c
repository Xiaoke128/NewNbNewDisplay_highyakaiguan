#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "ds18b20.h"
#include "hw_timer.h"
#include "Init.h"
#include "hw_uart.h"
#include "Isl1208.h"
#include "wh_nb73.h"
#include "modbus.h"
//#define USART0_REMAP
unsigned char temp[5] = " 00.0";
static void CheckLedNormal(void);

static void CheckLedNormal(void)
{
	if(NbInfo.AlarmFlags.val == 0)
	{
		NbInfo.AlarmSendOut = 0;
		NbInfo.DataLedStatus = DATA_LED_STATUS_NORMAL;
	}
}

int main(void)
{
		uint8_t ch = 0;
	
		MCU_Init();//system init
    while(1){
		if(systemFlag.bit.Time1ms)
		{
			systemFlag.bit.Time1ms = 0;
#ifndef DEBUG
			WdgtFeed();//watch dog feed
#endif
			DS18B20GetIntTemp();//Get internal tempture
			GetChannel3_Status();
			Cj2301_Init();
			Cj2301_Read();
			//DS18B20GetExtTemp();//get external tempture
			GetExitPinVal();//get the channal 1 ~ 3 pin level
			SignalCheck();//close or open knife action check
			GetRtcTime();//get rtc time
			
			CheckLedNormal();//check the led normal
			//GetCommunMode();//get the mode, modbus or NBIOT
			StoreInfoCheck();//check whether need to store info
			NB_Control();//nb module control
			ModbusProtocol();
			ProtocolTask();
			//FlashFunTest();
			//CommunSlaveFun();
			while(CommunCheck())
			{
					ch = CommunGetData();		
					if(systemFlag.bit.BypassMode)
					{
							HostUartSendBuf(&ch, 1);
					}
					else
					{
							ParsePro(ch);
					}
			}
		}
#if 0
		//get character from host uart, can add some action to do test
		if(HostCheck())
		{
			ch = HostGetData();
			buf[index++] = ch;
			if(index > 2)
			{
				if((buf[index - 2] == '\r') && (buf[index - 1] == '\n'))
				{
					//NbUartSendBuf(buf, index);
					//index = 0;
#if 1
					if(strstr((char *)buf, "send\r\n") != NULL)//send data package for test, also can add other action
					{
						//NbInfo.InfoFlags.bit.IntervalSend = 1;
						Isl1208GetTime();
						StoreInfoAdd();
					}
					if(strstr((char *)buf, "temp:") != NULL)
					{
						for(i = 0; i < index; i++)
						{
							if(buf[i] == ':')
							{
								break;
							}
						}
						i++;
						NbInfo.ConfData.AbnormalTem = buf[i++] - '0';
						while(buf[i] != '\r')
						{
							NbInfo.ConfData.AbnormalTem *= 10;
							NbInfo.ConfData.AbnormalTem += (buf[i] - '0');
							i++;
						}
						StoreConfUpdate();
					}
					memset(buf, 0, sizeof(buf));
					index = 0;
#endif
				}
			}
		}
		//CheckFun();
#endif
    }
}
