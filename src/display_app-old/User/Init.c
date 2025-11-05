#include "Init.h"
SystemFlag systemFlag;
StoreInfoStr StoreInfo;

char Version[] = "V1.04";
static void GpioInit(void);
static void VarInit(void);

static void GpioInit(void)
{
	gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED1_PIN);//LED1
	gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED2_PIN);//LED2
	gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED3_PIN);//LED3
	gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED4_PIN);//LED4
	
	//NB_RESET_HIGH;
	//gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	//GPIO_BOP(GPIOB) = GPIO_PIN_1;
	
	NET_LED_OFF;
	POWER_LED_ON;
	DATA_LED_NORMAL;
}

void LedToggle(void)
{
	static uint8_t flag = 0;
	static uint16_t count = 0;
	
	count++;
	if(count >= 500)
	{
		count = 0;
		if(flag)
		{
			flag = 0;
			GPIO_BOP(GPIOB) = GPIO_PIN_1;
		}
		else
		{
			flag = 1;
			GPIO_BC(GPIOB) = GPIO_PIN_1;
		}
	}
}

static void VarInit(void)
{
	systemFlag.val = 0;
	
	GUI_Func = DeviceStartFun;
	modbusValInit();
	WlVarInit();
	DisplayMainProInit();
	PakStuctInit();
}

static void RcuClockInit(void)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOB);//GPIOB CLOCK
	rcu_periph_clock_enable(RCU_GPIOA);//GPIOA CLOCK
	rcu_periph_clock_enable(RCU_USART0);//UART0 CLOCK
	gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
	//rcu_periph_clock_enable(RCU_USART1);//UART1 CLOCK
	rcu_periph_clock_enable(RCU_USART2);//UART2 CLOCK
	//gpio_pin_remap_config(GPIO_SPI0_REMAP, DISABLE);//SPI0 REMAP
	rcu_periph_clock_enable(RCU_SPI0);//SPI0 CLOCK
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
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
	FMC_StoreInfoInit();
	VarInit();//var init
	GpioInit();//IO init
	UartInit();//UART init
	printf("Display board app\r\nBuild Time: %s %s\r\nVersion: %s\r\n", __DATE__, __TIME__, Version);//print build time and date
	Timer_1ms_Init();//1ms timer init
	SignalTimerInit();//action timer init
	ButtonInit();
	//WdgtInit();//watch dog init
	OledInit();
}
