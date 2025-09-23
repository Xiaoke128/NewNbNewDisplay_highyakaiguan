#include "cj2301.h"
//0:不存在cj2301
//1:存在cj2301

static float waterTable[][13] = {
	{-3.0, -1.4, 0.1,  1.4,  2.6,  3.7,  4.8,  5.8,  6.7,  7.6,  8.4,  9.2,  10.0},
	{-2.1, -0.5, 1.0,  2.3,  3.5,  4.7,  5.7,  6.7,  7.7,  8.6,  9.4,  10.2, 11.0},
	{-1.2, 0.5,  1.9,  3.3,  4.5,  5.6,  6.7,  7.7,  8.7,  9.6,  10.4, 11.2, 12.0},
	{-0.3, 1.4,  2.8,  4.2,  5.4,  6.6,  7.7,  8.7,  9.6,  10.5, 11.4, 12.2, 13.0},
	{0.6,  2.3,  3.8,  5.1,  6.4,  7.5,  8.6,  9.6,  10.6, 11.5, 12.4, 13.2, 14.0},
	{1.5,  3.2,  4.7,  6.0,  7.3,  8.5,  9.6,  10.6, 11.6, 12.5, 13.4, 14.2, 15.0},
	{2.4,  4.1,  5.6,  7.0,  8.2,  9.4,  10.5, 11.6, 12.6, 13.5, 14.4, 15.2, 16.0},
	{3.3,  5.0,  6.5,  7.9,  9.2,  10.4, 11.5, 12.5, 13.5, 14.5, 15.3, 16.2, 17.0},
	{4.2,  5.9,  7.4,  8.8,  10.1, 11.3, 12.5, 13.5, 14.5, 15.4, 16.3, 17.2, 18.0},
	{5.1,  6.8,  8.4,  9.8,  11.1, 12.3, 13.4, 14.5, 15.5, 16.4, 17.3, 18.2, 19.0},
	{6.0,  7.7,  9.3,  10.7, 12.0, 13.2, 14.4, 15.4, 16.4, 17.4, 18.3, 19.2, 20.0},
	{6.9,  8.6,  10.2, 11.6, 12.9, 14.2, 15.3, 16.4, 17.4, 18.4, 19.3, 20.2, 21.0},
	{7.8,  9.5,  11.1, 12.6, 13.9, 15.1, 16.3, 17.4, 18.4, 19.4, 20.3, 21.2, 22.0},
	{8.7,  10.4, 12.0, 13.5, 14.8, 16.1, 17.2, 18.3, 19.4, 20.3, 21.3, 22.2, 23.0},
	{9.6,  11.3, 12.9, 14.4, 15.8, 17.0, 18.2, 19.3, 20.3, 21.3, 22.3, 23.1, 24.0},
	{10.5, 12.2, 13.9, 15.3, 16.7, 18.0, 19.1, 20.3, 21.3, 22.3, 23.2, 24.1, 25.0},
	{11.4, 13.2, 14.8, 16.3, 17.6, 18.9, 20.1, 21.2, 22.3, 23.3, 24.2, 25.1, 26.0},
	{12.3, 14.1, 15.7, 17.2, 18.6, 19.9, 21.1, 22.2, 23.3, 24.3, 25.2, 26.1, 27.0},
	{13.1, 15.0, 16.6, 18.1, 19.5, 20.8, 22.0, 23.2, 24.2, 25.2, 26.2, 27.1, 28.0},
	{14.0, 15.9, 17.5, 19.0, 20.4, 21.8, 23.0, 24.1, 25.2, 26.2, 27.2, 28.1, 29.0},
	{14.9, 16.8, 18.4, 20.0, 21.4, 22.7, 23.9, 25.1, 26.2, 27.2, 28.2, 29.1, 30.0},
	{15.8, 17.7, 19.4, 20.9, 22.3, 23.6, 24.9, 26.0, 27.1, 28.2, 29.2, 30.1, 31.0},
	{16.7, 18.6, 20.3, 21.8, 23.3, 24.6, 25.8, 27.0, 28.1, 29.2, 30.1, 31.1, 32.0},
	{17.6, 19.5, 21.2, 22.8, 24.2, 25.5, 26.8, 28.0, 29.1, 30.1, 31.1, 32.1, 33.0},
	{18.5, 20.4, 22.1, 23.7, 25.1, 26.5, 27.7, 28.9, 30.1, 31.1, 32.1, 33.1, 34.0},
	{19.4, 21.3, 23.0, 24.6, 26.1, 27.4, 28.7, 29.9, 31.0, 32.1, 33.1, 34.1, 35.0},
	{20.3, 22.2, 23.9, 25.5, 27.0, 28.4, 29.7, 30.9, 32.0, 33.1, 34.1, 35.1, 36.0},
	{21.2, 23.1, 24.8, 26.5, 27.9, 29.3, 30.6, 31.8, 33.0, 34.1, 35.1, 36.1, 37.0},
	{22.0, 24.0, 25.8, 27.4, 28.9, 30.3, 31.6, 32.8, 33.9, 35.0, 36.1, 37.1, 38.0},
	{22.9, 24.9, 26.7, 28.3, 29.8, 31.2, 32.5, 33.7, 34.9, 36.0, 37.1, 38.0, 39.0},
	{23.8, 25.8, 27.6, 29.2, 30.7, 32.2, 33.5, 34.7, 35.9, 37.0, 38.0, 39.0, 40.0}
};

static uint8_t Cj2301CheckConnect(void);

static uint8_t Cj2301CheckConnect(void)
{
		uint8_t retry = 0;
		
		//EXT_DS18B20_IO_OUT(); //SET PA0 OUTPUT
		//GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;
		//delay_1ms(2000);
		GPIO_BC(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; //拉低DQ
		delay_1us(1000);
		GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;
		EXT_DS18B20_IO_IN();//SET PA0 INPUT	
		while ((SET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		retry = 0;
		while ((RESET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		if(retry>=10)
		{
				//delay_1us(80);
				//return 1;
				systemFlag.bit.Cj2301_TempHumi = 1;
		}
		EXT_DS18B20_IO_OUT(); //SET PA0 OUTPUT
		GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;
		return 0;
}

void Cj2301_Init(void)
{
		static uint8_t step = 0;
		static uint16_t count = 0;
		
		if(systemFlag.bit.Cj2301_Init)
		{
				return;
		}
		switch(step)
		{
			case 0:
				count++;
				if(count >= 3000)
				{
						count = 0;
						step++;
				}
			break;
			case 1:
				Cj2301CheckConnect();
				systemFlag.bit.Cj2301_Init = 1;
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
					if(systemFlag.bit.Cj2301_TempHumi)
					{
							TX_ENABLE;
							printf("Find CJ2301!\r\n");
					}
					else
					{
							TX_ENABLE;
							printf("Not Find CJ2301!\r\n");
					}	
				}
			break;
			default:
				
			break;
		}
				
}

uint8_t ReadBit(void)
{
		uint8_t low_count = 0, high_count = 0;
			
		while ((RESET==EXT_DS18B20_DQ_IN)&&low_count<200)//读取代码未修改
		{
			low_count++;
			delay_1us(1);
		};
		while ((SET==EXT_DS18B20_DQ_IN)&&high_count<200)//读取代码未修改
		{
					high_count++;
					delay_1us(1);
		};
		if(low_count > high_count)
		{
				return 0;
		}
		else
		{
				return 1;
		}
}

uint8_t ReadByte(void)
{
		uint8_t ret = 0, i = 0, response = 0;
	
		for(i = 0; i < 8; i++)
		{
				response = ReadBit();
				ret += (response << (7 - i));
		}
		return ret;
}

void Cj2301_StartReadTempHumi(void)
{
		uint8_t retry = 0;
		GPIO_BC(EXT_BS18B20_PORT) = EXT_BS18B20_PIN; //拉低DQ
		delay_1us(1000);
		GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;
		EXT_DS18B20_IO_IN();//SET PA0 INPUT	
		while ((SET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		retry = 0;
		while ((RESET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
		retry = 0;
		while ((SET==EXT_DS18B20_DQ_IN)&&retry<200)//读取代码未修改
		{
			retry++;
			delay_1us(1);
		};
}

void Cj2301_EndRead(void)
{
		EXT_DS18B20_IO_OUT(); //SET PA0 OUTPUT
		GPIO_BOP(EXT_BS18B20_PORT) = EXT_BS18B20_PIN;
}

static void GetWaterTemp(void)
{
		uint16_t ret = NbInfo.Cj2301Temp / 10;
		uint8_t i = 0, j = 0;
		
		i = ret - 10;
		j = (NbInfo.Cj2301Humi / 10 - 40)/ 5;
	
		if(j < 12)
		{
				NbInfo.waterTemp = waterTable[i][j] + (waterTable[i][j + 1] - waterTable[i][j]) / 5 * (NbInfo.Cj2301Humi / 10 % 5);
		}
		else{
			NbInfo.waterTemp = waterTable[i][j];
		}
}

void Cj2301_ReadTempHumi(void)
{
		uint8_t buf[5] = {0};
		uint8_t i = 0;
		
		Cj2301_StartReadTempHumi();
		for(i = 0; i < 5; i++)
		{
				buf[i] = ReadByte();
		}
		Cj2301_EndRead();
		NbInfo.Cj2301Humi = (uint16_t)(buf[0] << 8) + buf[1];
		NbInfo.Cj2301Temp = (uint16_t)(buf[2] << 8) + buf[3];
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("CJ2301 tempture/humiduty: %d %d\r\n", NbInfo.Cj2301Temp / 10, NbInfo.Cj2301Humi / 10);
		}
		if((NbInfo.Cj2301Temp >= 100 && NbInfo.Cj2301Temp <= 400) && (NbInfo.Cj2301Humi >= 400) && (NbInfo.Cj2301Humi <= 1000))
		{
				GetWaterTemp();
		}
}

void Cj2301_Read(void)
{
		if(systemFlag.bit.RunCj2301 && systemFlag.bit.Cj2301_TempHumi)
		{
				systemFlag.bit.RunCj2301 = 0;
				Cj2301_ReadTempHumi();
		}
}

