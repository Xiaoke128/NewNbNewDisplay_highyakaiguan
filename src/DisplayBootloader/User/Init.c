#include "Init.h"
SystemFlag systemFlag;

static char Version[] = "V0.90";
static void GpioInit(void);
static void VarInit(void);

static void GpioInit(void)
{
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);//MODE1
	gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED2_PIN);//LED2
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
	gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);//I2C0 REMAP
	rcu_periph_clock_enable(RCU_USART0);//UART0 CLOCK
	//rcu_periph_clock_enable(RCU_USART1);//UART1 CLOCK
	rcu_periph_clock_enable(RCU_USART2);//UART2 CLOCK
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
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
	printf("Bootloader for GD32F103C8 Display board\r\nBuild Time: %s %s\r\nVersion: %s\r\n", __DATE__, __TIME__, Version);//print build time and date
	Timer_1ms_Init();//1ms timer init
	SignalTimerInit();//action timer init
	//WdgtInit();//watch dog init
}
