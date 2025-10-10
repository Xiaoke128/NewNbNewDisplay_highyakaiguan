#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "hw_timer.h"
#include "Init.h"
#include "hw_uart.h"
//#define USART0_REMAP

int main(void)
{
		MCU_Init();//system init

		//NVIC_SystemReset();
    while(1){
				if(systemFlag.bit.Time1ms)
				{
					systemFlag.bit.Time1ms = 0;
					ButtonScan();
					GUI_Func();
					//LedToggle();
					GetWirelessTempTask();
					DisplayMainProTask();
#if 0
					if(CommunCheck())
					{
							ch = CommunGetData();
							NbUartSendBuf(&ch, 1);
					}
					if(NbCheck())
					{
							ch = NbGetData();
							CommunUartSendBuf(&ch, 1);
					}
#endif
				}
    }
}
