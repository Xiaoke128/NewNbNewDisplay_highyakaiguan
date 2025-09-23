#include "Isl1208.h"

RtcTime TimeStr;
//write data to reg
static void Isl1208Write(uint8_t reg, uint8_t data)
{
	//while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
	//};
	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
	/* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, I2C0_SLAVE_ADDRESS7, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	/* data transmission */
    i2c_data_transmit(I2C0, reg);
    /* wait until the TBE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	/* data transmission */
	i2c_data_transmit(I2C0, data);
	/* wait until the TBE bit is set */
	while(!i2c_flag_get(I2C0, I2C_FLAG_TBE)){
	};
	/* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
	/* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0) & 0x0200);
}

//read data from reg
static uint8_t Isl1208Read(uint8_t reg)
{
	uint8_t data;
	
	//while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)){
	//};
	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
	/* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, I2C0_SLAVE_ADDRESS7, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	/* data transmission */
    i2c_data_transmit(I2C0, reg);
	/* wait until the TBE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	/* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
	/* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
	 /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, I2C0_SLAVE_ADDRESS7, I2C_RECEIVER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
	/* wait until the second last data byte is received into the shift register */
    //while(!i2c_flag_get(I2C0, I2C_FLAG_BTC));
    /* disable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);
	/* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
    /* read a data from I2C_DATA */
    data = i2c_data_receive(I2C0);
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(I2C0);
	/* wait until the stop condition is finished */
    while(I2C_CTL0(I2C0) & 0x0200);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
	
	return data;
}
//byte change to BCD
uint8_t ByteToBcd(uint8_t byteData)
{
	return (((byteData / 10 ) << 4) + ((byteData % 10) & 0x0F));
}

//BCD change to byte
uint8_t BcdToByte(uint8_t bcdData)
{
	return (0xFF & (bcdData >> 4)) * 10 + (0x0F & bcdData);
}

//get time from ISL1208
void Isl1208GetTime(void)
{
	TimeStr.second = BcdToByte(Isl1208Read(SECOND_REGISTER));
	TimeStr.minute = BcdToByte(Isl1208Read(MINUTE_REGISTER));
	TimeStr.hour = BcdToByte(Isl1208Read(HOUR_REGISTER) & 0x7F);
	TimeStr.date = BcdToByte(Isl1208Read(DATE_REGISTER));
	TimeStr.mouth = BcdToByte(Isl1208Read(MOUTH_REGISTER));
	TimeStr.year = BcdToByte(Isl1208Read(YEAR_REGISTER)) + 2000;
	NbInfo.uTime = UTC_to_Unix(TimeStr);
}
//ISL1208 INIT
void Isl1208Init(void)
{
	uint8_t status_data = 0;
	
	status_data = Isl1208Read(STATUS_REGISTER);
	if(status_data != ISL_INIT_STATUS)//if ISL1208 not lost power, no need init
	{
		Isl1208Write(STATUS_REGISTER, ISL_INIT_STATUS);//auto reset,write RTC enable
		//data = Isl1208Read(HOUR_REGISTER);
		//data |= HOUR_24_FORMAT;
		//Isl1208Write(HOUR_REGISTER, data);//24h format, start count
		
		//RtcSetTime(TimeStr);
	}
	Isl1208GetTime();
	if(TimeStr.year < 2020)
	{
		TimeStr.year = 2020;
		TimeStr.mouth = 11;
		TimeStr.date = 20;
		TimeStr.hour = 1;
		TimeStr.minute = 0;
		TimeStr.second = 0;
		RtcSetTime(TimeStr);
	}
}
//every 10 seconds to read time
void GetRtcTime(void)
{
	static uint32_t count = 0;
	
	count++;
	if((count % 1000) == 0)//get unix time every second
	{
		Isl1208GetTime();
		if(((NbInfo.uTime - NbInfo.ReportStartUtime) % (NbInfo.ConfData.ReportInterval * 60 * 60) == 0) && ((NbInfo.uTime - NbInfo.ReportStartUtime) != 0))//interval attach	
		//if(((NbInfo.uTime - NbInfo.ReportStartUtime) % (2 * 60) == 0) && (!NbInfo.InfoFlags.bit.CheckDataRes))//test 2 minutes send once
		{
			//NbInfo.InfoFlags.bit.IntervalSend = 1;
			Isl1208GetTime();
			if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
			{
				StoreInfoAdd();
			}
		}			
	}
	if(count >= 10000)//10 seconds printf once
	{
		count = 0;		
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("RTC Time is: %d/%d/%d %d:%d:%d\r\n", TimeStr.year, TimeStr.mouth, TimeStr.date, TimeStr.hour, TimeStr.minute, TimeStr.second);
		}
	}
}

//set time to RTC chip
void RtcSetTime(RtcTime time)
{
	Isl1208Write(SECOND_REGISTER, ByteToBcd((uint8_t)time.second));
	Isl1208Write(MINUTE_REGISTER, ByteToBcd((uint8_t)time.minute));
	Isl1208Write(HOUR_REGISTER, ByteToBcd((uint8_t)(time.hour)) | HOUR_24_FORMAT);
	Isl1208Write(DATE_REGISTER, ByteToBcd((uint8_t)time.date));
	Isl1208Write(MOUTH_REGISTER, ByteToBcd((uint8_t)time.mouth));
	Isl1208Write(YEAR_REGISTER, ByteToBcd((uint8_t)(time.year - 2000)));
}

//UTC time change to unix time
uint32_t UTC_to_Unix(RtcTime time)
{
	struct tm timeTemp;
	uint32_t timeUnix;
	
	timeTemp.tm_year = time.year - 1900;
	timeTemp.tm_mon = time.mouth - 1;
	timeTemp.tm_mday = time.date;
	timeTemp.tm_hour = time.hour;
	timeTemp.tm_min = time.minute;
	timeTemp.tm_sec = time.second;
	timeUnix = mktime(&timeTemp) - 8 * 60 * 60;
	
	
	return timeUnix;
}

//unix time change to UTC time
//	xtime:unix time
//	*time:point to UTC time
void Unix_to_UTC(uint32_t xtime, RtcTime *time)
{
	struct tm *timeTemp;
	
	xtime += 8 * 60 * 60;//benjing time need add 8 hours
	timeTemp = localtime(&xtime);
	time->year = timeTemp->tm_year + 1900;
	time->mouth = timeTemp->tm_mon + 1;//0 ~ 11 need add 1
	time->date = timeTemp->tm_mday;
	time->hour = timeTemp->tm_hour;
	time->minute = timeTemp->tm_min;
	time->second = timeTemp->tm_sec;
	if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
	{
			TX_ENABLE;
			printf("UTC time is: %d/%d/%d %d:%d:%d\r\n", time->year, time->mouth, time->date, time->hour, time->minute, time->second);
	}
}
