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
	
		TX_ENABLE;
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

int main(void)
{
		uint32_t *bootFlag = (uint32_t *)APP_BOOT_FLAG_ADDR;
		uint8_t ch = 0;
		uint8_t ShowBuf[] = "UART2 is for FW upgrade!";
	
		MCU_Init();//system init
		if((GET_MODE1_VAL != 0) && (*bootFlag != STAY_BOOT_FLAG_VAL))
		{
				RunApp(APP_ADDRESS);
		}
		TX_ENABLE;
		printf("Stay in loader for pin value or flag! Mode1 Pin:%d,  Flag:0x%X\r\n", GET_MODE1_VAL, *bootFlag);
		CommunUartSendBuf(ShowBuf, sizeof(ShowBuf));
		//RunApp(APP_ADDRESS);
    while(1){
				if(HostCheck())
				{
						ch = HostGetData();
						//HostUartSendBuf(&ch, 1);
						ReceParse(ch);
				}
    }
}
