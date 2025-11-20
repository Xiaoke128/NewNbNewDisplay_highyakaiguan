#include "Init.h"
SystemFlag systemFlag;
SignalInfo SigInfo;
StoreInfoStr StoreInfo;
BackUpStr BackUpInfo;

char Version[] = "V1.04H";
static void GpioInit(void);
static void VarInit(void);

void BackUpAdd(void)
{
	uint8_t i = 0;
	if(BackUpInfo.backupNum < 6)
	{
		memcpy(&BackUpInfo.Info[BackUpInfo.backupNum], &StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum], sizeof(InfoStr));
		BackUpInfo.backupNum++;
	}
	else
	{
		for(i = 0; i < 5; i++)
		{
			memcpy(&BackUpInfo.Info[i], &BackUpInfo.Info[i + 1], sizeof(InfoStr));
		}
		memcpy(&BackUpInfo.Info[5], &StoreInfo.Info[StoreInfo.NumInfo - NbInfo.SendPackNum], sizeof(InfoStr));
	}
}

void BackUpClearOne(void)
{
	uint8_t i = 0;
	if(BackUpInfo.backupNum > 0)
	{
		for(i = 0; i < 6; i++)
		{
			memcpy(&BackUpInfo.Info[i], &BackUpInfo.Info[i + 1], sizeof(InfoStr));
		}
		BackUpInfo.backupNum--;
	}
}

static void GpioInit(void)
{
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	GPIO_BOP(GPIOA) = GPIO_PIN_11;
	
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	GPIO_BOP(GPIOA) = GPIO_PIN_15;
	
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	GPIO_BOP(GPIOB) = GPIO_PIN_7;
	delay_1ms(10);
    /* enable the led clock */
    //rcu_periph_clock_enable(RCU_GPIOC);//使能GPIOC的时钟
	//rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOC的时钟
	//rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOC的时钟
    /* configure led GPIO port */ 
    //gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);//配置IO口参数，GPIOC,推挽输出,50M，PC13
	//gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED1_PIN);//LED1
	//gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED2_PIN);//LED2
	//gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED3_PIN);//LED3
	//gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED4_PIN);//LED4
	gpio_init(DE_485_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DE_485_PIN);//DE_485
	gpio_init(NB_POWER_CTR_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, NB_POWER_CTR_PIN);//NB POWER cotrol
	//NB_POWER_ON;
	gpio_init(NB_RESET_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, NB_RESET_PIN);//NB RESET pin cotrol
	gpio_init(MODE_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MODE1_PIN);//MODE1
	gpio_init(MODE_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MODE2_PIN);//MODE1
	
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);//hostwake up
	
	
	
	//NB_RESET_HIGH;
	NB_RESET_LOW;
	TX_ENABLE;
	if(GET_MODE2_VAL)
	{
			systemFlag.bit.ModbusOrNbiot = COMMUNICATE_MODBUS;
	}
	else
	{
			systemFlag.bit.ModbusOrNbiot = COMMUNICATE_NBIOT;
	}
}

static void VarInit(void)
{
	systemFlag.val = 0;
	//systemFlag.bit.Run18B20Inter = 1;
	//systemFlag.bit.Run18B20Exter = 1;
	SigInfo.Count = 0;
	SigInfo.KnifeFlag.val = 0;
	SigInfo.UseCount = 0;
	SigInfo.KnifeCloseTemp = 0;
	
#if 0
	//default time 2020/11/20 1:00:00
	TimeStr.year = 2020;
	TimeStr.mouth = 11;
	TimeStr.date = 20;
	TimeStr.hour = 1;
	TimeStr.minute = 0;
	TimeStr.second = 0;
#endif
	
	memset((void *)&NbInfo, 0, sizeof(NbInfo));
	memset(&BackUpInfo, 0, sizeof(BackUpInfo));
	//NbInfo.CurrentTick = rand() % 500 + 1;//random tick
	NbInfo.Btype = SEND_REGISTER_BUF;
	NbInfo.ConfData.TemCollectInterval = 5;//default 5 seconds get tempture
	NbInfo.ConfData.TimeTick = rand() % 500 + 1;//random tick 1 ~ 500
	NbInfo.ConfData.ReportInterval = 4;//4 hours report 1 data package	
	NbInfo.ConfData.AbnormalTem = 60;//default tempture
	NbInfo.ConfData.AbnormalCloseTime = 3500;
	NbInfo.ConfData.AbnormalOpenTime = 3500;
	NbInfo.ConfData.CloseAlramTime = 1500;
	NbInfo.ConfData.OpenAlarmTime = 1500;
	NbInfo.ConfData.AbnormalHumi = 95;
	NbInfo.ReportStartUtime = UTC_to_Unix(TimeStr) + (uint32_t)(NbInfo.ConfData.TimeTick * 60);//report start time
	
	systemFlag.bit.Run18B20Inter = 1;//get tempture when power on
	systemFlag.bit.Run18B20Exter = 1;//get tempture when power on
	
	//NB_POWER_ON;//nb power on
	NbResetEnable();//nb need to reset when power on
	
	PakStuctInit();
	modbusValInit();
}

static void RcuClockInit(void)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOB);//GPIOB CLOCK
	rcu_periph_clock_enable(RCU_GPIOA);//GPIOA CLOCK
	rcu_periph_clock_enable(RCU_USART0);//UART0 CLOCK
	rcu_periph_clock_enable(RCU_USART1);//UART1 CLOCK
	rcu_periph_clock_enable(RCU_USART2);//UART2 CLOCK
	gpio_pin_remap_config(GPIO_I2C0_REMAP, ENABLE);//I2C0 REMAP
	rcu_periph_clock_enable(RCU_I2C0);//I2C0 CLOCK
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
	gpio_pin_remap_config(GPIO_SPI0_REMAP, ENABLE);//SPI0 REMAP
	rcu_periph_clock_enable(RCU_SPI0);//SPI0 CLOCK
	rcu_periph_clock_enable(RCU_ADC0);
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_SPI0);
}

void MCU_Init(void)
{
#ifndef DEBUG
	__disable_irq();
	nvic_vector_table_set(NVIC_VECTTAB_FLASH, APP_VETC_OFFSET);
	__enable_irq();
#endif
	systick_config();//SYSTEM tick init
	RcuClockInit();//RCU clock init
	nvic_config();//Interrupt init
	VarInit();//var init
	FMC_StoreInfoInit();//Store info init
	GpioInit();//IO init
	UartInit();//UART init
	TX_ENABLE;
	printf("Build Time: %s %s\r\nVersion: %s\r\nIP & Port:10.3.226.194,9004\r\n", __DATE__, __TIME__, Version);//print build time and date
	Timer_1ms_Init();//1ms timer init
	SignalTimerInit();//action timer init
	AdcInit();
	AdcTimerInit();
	ExitInit();//exit interrupt init
	I2C_Init();//I2C init
	Isl1208Init();//ISL1208 Init
	DS18B20_Init();	//DS18B20 init
	
	NandFlashInit();
	//NandFlashInit();//NAND flash init
#ifndef DEBUG
	WdgtInit();//watch dog init
#endif
	if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_MODBUS)
	{
		TX_ENABLE;
		printf("Run in modbus mode!\r\n");
	}
	else
	{
		TX_ENABLE;
		printf("Run in NBIOT mode!\r\n");
	}
}
