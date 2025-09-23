#ifndef WH_NB73_H
#define WH_NB73_H
#include <stdio.h>
#include <string.h>
#include "gd32f10x.h"
#include "Init.h"
#include "hw_fmc.h"

#define AT_COM_RES_TIMEOUT				1000
#define RES_UNIX_TIME_OFFSET			0x13
#define RES_DATA_COMMAND_TYPE_OFFSET	0x0A
#define RES_COMMAND_TYPE_MASK			0x80
#define COLLECT_INTERVAL_OFFSET			0x12
#define TIME_TICK_OFFSET				0x14
#define REPORT_PERIOD_OFFSET			0x16
#define CLOSE_ALARM_OFFSET				0x17
#define OPEN_ALARM_OFFSET				0x1B
#define CLOSE_ABNORMAL_OFFSET			0x1F
#define OPEN_ABNORMAL_OFFSET			0x23
#define TEMP_ALARM_OFFSET				0x27
#define HUMI_ALARM_OFFSET				0x28
#define TEMP_DIFF_OFFSET				0x29

#define NET_LED_STATUS_OFF				0x00
#define NET_LED_STATUS_FLASH_200MS		0x01
#define NET_LED_STATUS_FLASH_1000MS		0x02

#define DATA_LED_STATUS_NORMAL			0x00
#define DATA_LED_STATUS_ABNORMAL_FLASH	0x01
#define DATA_LEN_STATUS_ALARM			0x02

//AT command response type
typedef enum{
	AT_RES_CHECKING,
	AT_RES_OK,
	AT_RES_ERROR,
	AT_RES_TIMEOUT,
}AT_ResType;

//Get info flag
typedef union{
	uint16_t val;
	struct{
		uint8_t		Init_App		:1;//0:INIT 1:app
		uint8_t		IMEI_Get		:1;//IMEI whether get
		uint8_t		CCID_Get		:1;//CCID whether get
		uint8_t		PCI_Get			:1;//PCI whether get
		uint8_t 	SocketCreate	:1;//Socket whether create
		uint8_t 	CheckRegRes		:1;//need to check register response
		uint8_t		SendRegister	:1;//send register data
		uint8_t		CheckDataRes	:1;//need to check data package response
		uint8_t		RegWaitHour		:1;//register no response, need to wait 1 hour.
		uint8_t		RefisterSuccess	:1;//register success
		//uint8_t		IntervalSend	:1;//interval send
		//uint8_t		OpenCloseActSend:1;//open or close action, need send data
		uint8_t 	ConfResSend		:1;//need to response configure package
		//uint8_t		TempAlarmSend	:1;//tempture alarm send
		uint8_t		SelfIpGet		:1;//whether getchar self IP
		//uint8_t		Chanel4_AlarmSend		:1;//channel 4 alarm send
		uint8_t 	Reserved		:4;
	}bit;
}NbGetInfoFlag;


//NB module action flag
typedef union{
	uint16_t val;
	struct{
		uint8_t		ModuleReset		:1;//need reset
		uint8_t		GetImei			:1;//need get imei
		uint8_t		GetCCID			:1;//need get ccid
		uint8_t		CreateSocket	:1;//need create a socket
		uint8_t		GetPCI			:1;//need get pci
		uint8_t		GetAttach		:1;//need get network status
		uint8_t		GetRSSI			:1;//need get rssi
		uint8_t		SendData		:1;//need send data
		uint8_t		RevData			:1;//need read data from server
		uint8_t		GetIP			:1;//need to get self IP
		uint8_t 	Reserved		:6;
	}bit;	
}NbRunInPro;

typedef union{
	uint8_t val;
	struct{
		uint8_t		OpenAlarm		:1;
		uint8_t		CloseAlarm		:1;
		uint8_t		ExterTempAlarm	:1;
		uint8_t		InterTempAlarm	:1;
		uint8_t		TempDiffAlarm	:1;
		uint8_t		Reserved		:3;
	}bit;
}AlarmFlag;

typedef enum{
	RUN_IN_NOTHING,
	RUN_IN_MODULE_RESET,
	RUN_IN_GET_IMEI,
	RUN_IN_GET_CCID,
	RUN_IN_CREATE_SOCKET,
	RUN_IN_GET_PCI,
	RUN_IN_GET_ATTACH,
	RUN_IN_GET_RSSI,
	RUN_IN_SEND_DATA,
	RUN_IN_REV_DATA,
	RUN_IN_ASYN_CHECK,
	RUN_IN_GET_IP,
}RunInApp;

//action type
typedef enum{
	SIGNAL_NO,
	SIGNAL_OPEN,
	SIGNAL_CLOSE,
}SignalActType;

//NB package type
typedef enum{
	SEND_REGISTER_BUF = 0x01,
	SEND_REVEIVE_BUF,
	SEND_DATA_BUF,
}BufType;

//Configure package parameters
typedef struct{
	uint16_t TemCollectInterval;//second, default 5 seconds
	uint16_t TimeTick;//minute, default random
	uint32_t CloseAlramTime;//100 Microsecond 
	uint32_t OpenAlarmTime;//100 Microsecond
	uint32_t AbnormalCloseTime;//100 Microsecond
	uint32_t AbnormalOpenTime;//100 Microsecond
	uint8_t ReportInterval;//hour, default 4 hours
	uint8_t AbnormalTem;//tempture alarm
	uint8_t TempDiffAlarmVal;//tempture different value
	uint8_t AbnormalHumi;
}ConfStr;

typedef struct{
	NbGetInfoFlag InfoFlags;
	NbRunInPro	AppPro;
	RunInApp 	RunInStep;
	AlarmFlag	AlarmFlags;
	uint8_t AlarmSendOut;
	uint8_t	SendPackNum;
	uint8_t SendDisplayPakNum;
	uint8_t IMEI_BYTE[16];
	uint8_t IMEI_BCD[8];
	uint8_t CCID_BYTE[20];
	uint8_t CCID_BCD[10];
	uint16_t PCI;
	uint8_t RSSI;
	uint8_t NetStatus;
	uint8_t Socket;
	uint32_t ReportStartUtime;
	uint32_t uTime;
	//uint16_t CurrentTick;
	uint8_t InterTemp;
	uint8_t ExterTemp;
	uint16_t Cj2301Temp;
	uint16_t Cj2301Humi;
	uint8_t NetLedStatus;
	uint8_t DataLedStatus;
	uint8_t Chanel4_AlarmFlag;
	uint8_t Channel3_Status;
	SignalActType Stype;//signal type
	uint32_t Stime;//signal time
	BufType Btype;//Send data type
	uint16_t NoAttachTime;//5 seconds
	float waterTemp;
	uint8_t OpenDelayFlag;
	uint8_t CloseDelayFlag;
	uint16_t OpenDelayCount;
	uint16_t CloseDelayCount;
	ConfStr ConfData;
}NbInfomation;

typedef struct{
	uint32_t ReportStartTime;
	ConfStr ServerConf;
	uint8_t SlaveAddr;
	uint8_t reerved;
	uint32_t BaudRateModbus;
	uint32_t CrcVal;
}StoreConfStr;


void NB_Control(void);
void NbResetEnable(void);
void GetCcidEnable(void);
void GetImeiEnable(void);
void GetPciEnable(void);
void GetAttchEnable(void);
void GetRssiEnable(void);
void CreateSocketEnable(void);
void SendDataEnable(void);
void GetIpEnable(void);

extern NbInfomation NbInfo;
extern StoreConfStr StoreConf;
#endif

