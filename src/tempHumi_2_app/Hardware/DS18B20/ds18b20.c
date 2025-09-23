#include "ds18b20.h"
#include "systick.h"	

float TempVal = 0;
static void CheckTempDiff(void);
//复位DS18B20
void DS18B20_Rst(TempType type)	   
{                 
	if(type == TEMP_INTER)
	{
		DS18B20_IO_OUT(); //SET PA0 OUTPUT
		GPIO_BC(BS18B20_PORT) = BS18B20_PIN; //拉低DQ
		delay_1us(750);    //拉低750us
		GPIO_BOP(BS18B20_PORT) = BS18B20_PIN; //DQ=1 
		delay_1us(15);     //15US
	}
	else
	{
		EXT_DS18B20_IO_OUT(); //SET PA0 OUTPUT
		GPIO_BC(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; //拉低DQ
		delay_1us(750);    //拉低750us
		GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; //DQ=1 
		delay_1us(15);     //15US
	}
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
unsigned char DS18B20_Check(TempType type) 	   
{   
	unsigned char retry=0;
	if(type == TEMP_INTER)
	{
		DS18B20_IO_IN();//SET PA0 INPUT	 
		while ((SET==DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};	 
		if(retry>=200)
			return 1;
		else retry=0;
		while ((SET!=DS18B20_DQ_IN)&&retry<240)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		if(retry>=240)
			return 1;	    
		return 0;
	}
	else
	{
		EXT_DS18B20_IO_IN();//SET PA0 INPUT	
		while ((SET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};	 
		if(retry>=200)
			return 1;
		else retry=0;
		while ((SET!=EXT_DS18B20_DQ_IN)&&retry<240)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		if(retry>=240)
			return 1;	    
		return 0;
	}
}
//从DS18B20读取一个位
//返回值：1/0
unsigned char DS18B20_Read_Bit(void) 			 // read one bit
{
    unsigned char data;
	DS18B20_IO_OUT();//SET PA0 OUTPUT
    GPIO_BC(BS18B20_PORT) = BS18B20_PIN; 
	delay_1us(2);
    GPIO_BOP(BS18B20_PORT) = BS18B20_PIN; 
	DS18B20_IO_IN();//SET PA0 INPUT
	delay_1us(12);
	if((SET==DS18B20_DQ_IN))data=1;//读取代码未修改
    else data=0;	 
    delay_1us(50);           
    return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
unsigned char DS18B20_Read_Byte(void)    // read one byte
{        
    unsigned char i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

//从DS18B20读取一个位
//返回值：1/0
unsigned char Ext_DS18B20_Read_Bit(void) 			 // read one bit
{
    unsigned char data;
	EXT_DS18B20_IO_OUT();//SET PA0 OUTPUT
    GPIO_BC(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; 
	delay_1us(2);
    GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; 
	EXT_DS18B20_IO_IN();//SET PA0 INPUT
	delay_1us(12);
	if((SET==EXT_DS18B20_DQ_IN))data=1;//读取代码未修改
    else data=0;	 
    delay_1us(50);           
    return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
unsigned char Ext_DS18B20_Read_Byte(void)    // read one byte
{        
    unsigned char i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=Ext_DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(unsigned char dat)     
 {             
    unsigned char j;
    unsigned char testb;
	DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            GPIO_BC(BS18B20_PORT)=BS18B20_PIN;// Write 1
            delay_1us(2);                            
            GPIO_BOP(BS18B20_PORT)=BS18B20_PIN;
            delay_1us(60);             
        }
        else 
        {
            GPIO_BC(BS18B20_PORT)=BS18B20_PIN;// Write 0
            delay_1us(60);             
            GPIO_BOP(BS18B20_PORT)=BS18B20_PIN;
            delay_1us(2);                          
        }
    }
}
 
//写一个字节到DS18B20
//dat：要写入的字节
void Ext_DS18B20_Write_Byte(unsigned char dat)     
 {             
    unsigned char j;
    unsigned char testb;
	EXT_DS18B20_IO_OUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            GPIO_BC(EXT_BS18B20_PORT)=EXT_BS18B20_PIN;// Write 1
            delay_1us(2);                            
            GPIO_BOP(EXT_BS18B20_PORT)=EXT_BS18B20_PIN;
            delay_1us(60);             
        }
        else 
        {
            GPIO_BC(EXT_BS18B20_PORT)=EXT_BS18B20_PIN;// Write 0
            delay_1us(60);             
            GPIO_BOP(EXT_BS18B20_PORT)=EXT_BS18B20_PIN;
            delay_1us(2);                          
        }
    }
}
//开始温度转换
void DS18B20_Start(TempType type)// ds1820 start convert
{   						               
    DS18B20_Rst(type);	   
	DS18B20_Check(type);	
	if(type == TEMP_INTER)
	{
		DS18B20_Write_Byte(0xcc);// skip rom
		DS18B20_Write_Byte(0x44);// convert
		//delay_1ms(750);
	}
	else
	{
		Ext_DS18B20_Write_Byte(0xcc);
		Ext_DS18B20_Write_Byte(0x44);
	}
} 
//初始化DS18B20的IO口 DQ 同时检测DS的存在   	 
void DS18B20_Init(void)
{    
	rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOB的时钟
    
    gpio_init(BS18B20_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BS18B20_PIN);//配置IO口参数，GPIOB,推挽输出,50M，PC0
	gpio_init(EXT_BS18B20_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, EXT_BS18B20_PIN);//配置IO口参数，GPIOB,推挽输出,50M，PC0

    GPIO_BOP(BS18B20_PORT) = BS18B20_PIN;//初始化IO口为高电平
	GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;//初始化IO口为高电平

	DS18B20_Rst(TEMP_INTER);
	if(!DS18B20_Check(TEMP_INTER))
	{
		systemFlag.bit.InterTemp = 1;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("Internal DS18B20 FIND!\r\n");
		}
	}
	else
	{
		systemFlag.bit.InterTemp = 0;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("Internal DS18B20 not FIND!\r\n");
		}
	}
#if 0
	DS18B20_Rst(TEMP_EXTER);
	if(!DS18B20_Check(TEMP_EXTER))
	{
		systemFlag.bit.ExterTemp = 1;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("External DS18B20 FIND!\r\n");
		}
	}
	else
	{
		systemFlag.bit.ExterTemp = 0;
		TX_ENABLE;
		printf("External DS18B20 not FIND!\r\n");
	}
#endif
}  
 
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
//1ms 执行一次函数
void DS18B20GetIntTemp(void)
{
	unsigned char temp;
    unsigned char TL,TH;
	short tem;
	static uint8_t step = 0;
	static uint16_t Count = 0;
	static bool PreTempNormal = TRUE, CurrentTempNormal = TRUE;
	TempType type = TEMP_INTER;
	static float PreTemp;//发送异常时的温度值
	static uint8_t BackNormal = 0, BackNormalCount = 0;
	
	
	if(systemFlag.bit.Run18B20Inter)
	{
		if(systemFlag.bit.InterTemp)
		{
			switch(step)
			{
			case 0:
				DS18B20_Start(type);// ds1820 start convert
				step++;
				break;
			case 1:
				Count++;
				if(Count >= 750)
				{
					Count = 0;
					step++;
				}
				break;
			case 2:
				step = 0;
				DS18B20_Rst(type);
				DS18B20_Check(type);		//DS18B20初始化	

				DS18B20_Write_Byte(0xcc);// skip rom
				DS18B20_Write_Byte(0xbe);// convert	    
				TL=DS18B20_Read_Byte(); // LSB   
				TH=DS18B20_Read_Byte(); // MSB

				if(TH>7)
				{
					TH=~TH;
					TL=~TL; 
					temp=0;//温度为负  
				}else temp=1;//温度为正	  	  
				tem=TH; //获得高八位
				tem<<=8;    
				tem+=TL;//获得底八位
				TempVal=(float)tem*0.0625;//转换 
				systemFlag.bit.Run18B20Inter = 0;	
				if(temp)
				{
					NbInfo.InterTemp = ((uint8_t)TempVal) + 100;
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("Internal temperature: %.1f\r\n", TempVal);
					}
					//RX_ENABLE;
				}
				else
				{
					NbInfo.InterTemp = 100 - ((uint8_t)TempVal);
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("Internal temperature: -%.1f\r\n", TempVal);
					}
					//RX_ENABLE;
				}
				if(((uint8_t)TempVal) >= StoreConf.ServerConf.AbnormalTem)//tempture alarm
				{					
					CurrentTempNormal = FALSE;		
					//DATA_LED_ALARM;					
				}
				else
				{					
					CurrentTempNormal = TRUE;
					//DATA_LED_NORMAL;
				}
				if(CurrentTempNormal != PreTempNormal)//正常和异常状态跳变时
				{
					if(!CurrentTempNormal)//变为异常时，发送异常数据，并记录异常温度值
					{
						if(BackNormalCount == 0)
						{
							//NbInfo.InfoFlags.bit.TempAlarmSend = 1;	
							NbInfo.AlarmFlags.bit.InterTempAlarm = 1;
							NbInfo.AlarmSendOut = 0;
							NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
							Isl1208GetTime();
							if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
							{
									StoreInfoAdd();				
							}							
						}
						PreTemp = TempVal;
						BackNormal = 0;
						BackNormalCount = 0;
					}
					else//变为正常状态，需满足6次
					{
						BackNormal = 1;
						BackNormalCount++;
					}
				}
				else//没有状态跳变时
				{
					if(!CurrentTempNormal)//一直为异常状态时
					{
						if(((PreTemp - TempVal) >= 5.0) || ((TempVal - PreTemp) >= 5.0))//温度变化超过5度时，再次发送异常数据
						{
							//NbInfo.InfoFlags.bit.TempAlarmSend = 1;															
							PreTemp = TempVal;
							Isl1208GetTime();
							if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
							{
									StoreInfoAdd();
							}
						}
					}
					else//一直为正常状态时
					{
						if(BackNormal)//检测6次连续正常，则发送一次数据
						{
							BackNormalCount++;
							if(BackNormalCount >= 6)
							{
								NbInfo.AlarmFlags.bit.InterTempAlarm = 0;
								NbInfo.AlarmSendOut = 0;
								BackNormal = 0;
								BackNormalCount = 0;
								//NbInfo.InfoFlags.bit.TempAlarmSend = 1;
								Isl1208GetTime();
								if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
								{
										StoreInfoAdd();
								}
							}
						}
					}
				}
				PreTempNormal = CurrentTempNormal;
				break;
			default:
				step = 0;
				break;		
			}
		}
	}
}


//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
//1ms 执行一次函数
void DS18B20GetExtTemp(void)
{
	unsigned char temp;
    unsigned char TL,TH;
	short tem;
	static uint8_t step = 0;
	static uint16_t Count = 0;
	static bool PreTempNormal = TRUE, CurrentTempNormal = TRUE;
	TempType type = TEMP_EXTER;
	static float PreTemp;//发送异常时的温度值
	static uint8_t BackNormal = 0, BackNormalCount = 0;
	
	if(systemFlag.bit.Run18B20Exter)
	{
		if(systemFlag.bit.ExterTemp)
		{
			switch(step)
			{
			case 0:
				DS18B20_Start(type);// ds1820 start convert
				step++;
				break;
			case 1:
				Count++;
				if(Count >= 750)
				{
					Count = 0;
					step++;
				}
				break;
			case 2:
				step = 0;
				DS18B20_Rst(type);
				DS18B20_Check(type);		//DS18B20初始化	
				Ext_DS18B20_Write_Byte(0xcc);
				Ext_DS18B20_Write_Byte(0xbe);
				TL=Ext_DS18B20_Read_Byte();
				TH=Ext_DS18B20_Read_Byte();

				if(TH>7)
				{
					TH=~TH;
					TL=~TL; 
					temp=0;//温度为负  
				}else temp=1;//温度为正	  	  
				tem=TH; //获得高八位
				tem<<=8;    
				tem+=TL;//获得底八位
				TempVal=(float)tem*0.0625;//转换 
				systemFlag.bit.Run18B20Exter = 0;	
				if(temp)
				{
					NbInfo.ExterTemp = ((uint8_t)TempVal) + 100;
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("External temperature: %.1f\r\n", TempVal);
					}
					//RX_ENABLE;
				}
				else
				{
					NbInfo.ExterTemp = 100 - ((uint8_t)TempVal);
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("External temperature: -%.1f\r\n", TempVal);
					}
					//RX_ENABLE;
				}
				if(((uint8_t)TempVal) >= StoreConf.ServerConf.AbnormalTem)
				{
					CurrentTempNormal = FALSE;		
					//DATA_LED_ALARM;					
				}
				else
				{
					CurrentTempNormal = TRUE;
					//DATA_LED_NORMAL;
				}
				if(CurrentTempNormal != PreTempNormal)//正常和异常状态跳变时
				{
					if(!CurrentTempNormal)//变为异常时，发送异常数据，并记录异常温度值
					{
						if(BackNormalCount == 0)
						{
							//NbInfo.InfoFlags.bit.TempAlarmSend = 1;
							NbInfo.AlarmFlags.bit.ExterTempAlarm = 1;
							NbInfo.AlarmSendOut = 0;
							NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
							Isl1208GetTime();
							if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
							{
									StoreInfoAdd();
							}
						}
						PreTemp = TempVal;
						BackNormal = 0;
						BackNormalCount = 0;
					}
					else//变为正常状态，需满足6次
					{
						BackNormal = 1;
						BackNormalCount++;
					}
				}
				else//没有状态跳变时
				{
					if(!CurrentTempNormal)//一直为异常状态时
					{
						if(((PreTemp - TempVal) >= 5.0) || ((TempVal - PreTemp) >= 5.0))//温度变化超过5度时，再次发送异常数据
						{
							//NbInfo.InfoFlags.bit.TempAlarmSend = 1;		
							PreTemp = TempVal;
							Isl1208GetTime();
							if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
							{
									StoreInfoAdd();
							}
						}
					}
					else//一直为正常状态时
					{
						if(BackNormal)//检测6次连续正常，则发送一次数据
						{
							BackNormalCount++;
							if(BackNormalCount >= 6)
							{
								NbInfo.AlarmFlags.bit.ExterTempAlarm = 0;
								NbInfo.AlarmSendOut = 0;
								BackNormal = 0;
								BackNormalCount = 0;
								//NbInfo.InfoFlags.bit.TempAlarmSend = 1;
								Isl1208GetTime();
								if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
								{
									StoreInfoAdd();
								}
							}
						}
					}
				}
				PreTempNormal = CurrentTempNormal;
				CheckTempDiff();
				break;
			default:
				step = 0;
				break;		
			}
		}
	}
}

static void CheckTempDiff(void)
{
	static bool PreStatus = FALSE, CurrStatus = FALSE;
	static uint8_t BackNormal = 0, BackNormalCount = 0;
	
	uint8_t CurrTempDiff = 0;
	
	if(NbInfo.InterTemp > NbInfo.ExterTemp)
	{
		CurrTempDiff = NbInfo.InterTemp - NbInfo.ExterTemp;
	}
	else
	{
		CurrTempDiff = NbInfo.ExterTemp - NbInfo.InterTemp;
	}
	if(CurrTempDiff >= StoreConf.ServerConf.TempDiffAlarmVal)
	{
		CurrStatus = TRUE;
	}
	else
	{
		CurrStatus = FALSE;
	}
	
	if(PreStatus != CurrStatus)
	{
		if(CurrStatus)
		{
			//NbInfo.InfoFlags.bit.TempAlarmSend = 1;	
			NbInfo.AlarmFlags.bit.TempDiffAlarm = 1;
			NbInfo.AlarmSendOut = 0;
			BackNormal = 0;
			BackNormalCount = 0;
			Isl1208GetTime();
			if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
			{
					StoreInfoAdd();
			}
		}
		else
		{
			BackNormal = 1;
			BackNormalCount++;
		}
	}
	else
	{
		if(!CurrStatus)
		{
			if(BackNormal)
			{
				if(CurrTempDiff <= (StoreConf.ServerConf.TempDiffAlarmVal - 2))
				{
					BackNormalCount++;
				}
				else
				{
					BackNormalCount = 0;
				}
				if(BackNormalCount >= 6)
				{
					NbInfo.AlarmFlags.bit.TempDiffAlarm = 0;
					NbInfo.AlarmSendOut = 0;
					BackNormal = 0;
					BackNormalCount = 0;
					//NbInfo.InfoFlags.bit.TempAlarmSend = 1;
					Isl1208GetTime();
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							StoreInfoAdd();
					}
				}
			}
		}
	}
	PreStatus = CurrStatus;
}
