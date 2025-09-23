#include "Init.h"
SystemFlag systemFlag;

static char Version[] = "V0.90";
static void GpioInit(void);
static void VarInit(void);

static void GpioInit(void)
{
	gpio_init(DE_485_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DE_485_PIN);//DE_485
	gpio_init(NB_RESET_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, NB_RESET_PIN);//NB RESET pin cotrol
	
	gpio_init(MODE_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MODE1_PIN);//MODE1
	gpio_init(MODE_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, MODE2_PIN);//MODE1
	
	NB_RESET_LOW;
	TX_ENABLE;
}

static void VarInit(void)
{
	systemFlag.val = 0;
	//systemFlag.bit.Run18B20Inter = 1;
	//systemFlag.bit.Run18B20Exter = 1;
	
	systemFlag.bit.Run18B20Inter = 1;//get tempture when power on
	systemFlag.bit.Run18B20Exter = 1;//get tempture when power on
	PakStuctInit();
}

static void RcuClockInit(void)
{
	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_GPIOB);//GPIOB CLOCK
	rcu_periph_clock_enable(RCU_GPIOA);//GPIOA CLOCK
	rcu_periph_clock_enable(RCU_USART0);//UART0 CLOCK
	rcu_periph_clock_enable(RCU_USART1);//UART1 CLOCK
	rcu_periph_clock_enable(RCU_USART2);//UART2 CLOCK
}

void MCU_Init(void)
{
	systick_config();//SYSTEM tick init
	RcuClockInit();//RCU clock init
	nvic_config();//Interrupt init
	FMC_StoreInfoInit();
	VarInit();//var init
	GpioInit();//IO init
	UartInit();//UART init
	TX_ENABLE;
	printf("Bootloader for GD32F103C8\r\nBuild Time: %s %s\r\nVersion: %s\r\n", __DATE__, __TIME__, Version);//print build time and date
	Timer_1ms_Init();//1ms timer init
	SignalTimerInit();//action timer init
	//WdgtInit();//watch dog init
}
