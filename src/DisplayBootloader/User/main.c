#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "hw_timer.h"
#include "Init.h"
#include "hw_uart.h"
#include "bootLoader.h"
//#define USART0_REMAP

pFunction JumpToApp;

void RunApp(uint32_t AppAddr)
{
		uint32_t JumpAddress = 0;
		uint8_t i = 0;
	
		printf("Jump to Application...\r\n");
		//rcu_system_clock_source_config(RCU_CKSYSSRC_IRC8M);
		__disable_irq();
		for(i = 0; i < 8; i++)
		{
				NVIC->ICER[i] = 0xFFFFFFFF;
				NVIC->ICPR[i] = 0xFFFFFFFF;
		}
		//nvic_vector_table_set(NVIC_VECTTAB_FLASH, APP_VETC_OFFSET);
		//__enable_irq();
		JumpAddress = *(__IO uint32_t*) (AppAddr + 4);
		JumpToApp = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) AppAddr);
		JumpToApp();  //
}

void LedFlash(void)
{
		static uint16_t count = 0;
		static bool state = FALSE;
		if(systemFlag.bit.Time1ms)
		{
				systemFlag.bit.Time1ms = 0;
				count++;
				if(count >= 300)
				{
						count = 0;
						if(state)
						{
								POWER_LED_ON;
						}
						else
						{
								POWER_LED_OFF;
						}
						state = !state;
				}
		}
}

int main(void)
{
		uint32_t *bootFlag = (uint32_t *)APP_BOOT_FLAG_ADDR;
		uint8_t ch = 0;
		char buf[] = "uart is ok";
	
		MCU_Init();//system init
		delay_1ms(1000);
#if 1
		if((GET_LOADER_PIN_VAL != 0) && (*bootFlag != STAY_BOOT_FLAG_VAL))
		{
				RunApp(APP_ADDRESS);
		}
#endif
		printf("Stay in loader for pin value or flag! Mode1 Pin:%d,  Flag:0x%X\r\n", GET_LOADER_PIN_VAL, *bootFlag);
		//RunApp(APP_ADDRESS);
		CommunUartSendBuf((uint8_t *)buf, 10);
    while(1){
				LedFlash();
				if(CommunCheck())
				{
						ch = CommunGetData();
						//HostUartSendBuf(&ch, 1);
						ReceParse(ch);
				}
    }
}
