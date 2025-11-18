


/**************************************************/
#include "hw_timer.h"

static uint16_t TempureInterval = 0;


//init a 1ms timer
void Timer_1ms_Init(void)
{
	timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER1);
    timer_deinit(TIMER1);
	
	/* TIMER0 configuration */
    timer_initpara.prescaler         = 107;//108M / 108 = 1M
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;// 1000 * 1 / 1000000 = 1ms
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);
	
	timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
}

//init a timer to check clsoe or open action time, 100us
void SignalTimerInit(void)
{
	timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);
	
	/* TIMER0 configuration */
    timer_initpara.prescaler         = 107;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 99;// 100 * 1 / 1000000 = 100us
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);
	
	timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);
    //timer_enable(TIMER2);
}

void AdcTimerInit(void)
{
	timer_parameter_struct timer_initpara;
	
	rcu_periph_clock_enable(RCU_TIMER3);
    timer_deinit(TIMER3);
	
	/* TIMER0 configuration */
    timer_initpara.prescaler         = 107;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 399;// 40 * 1 / 1000000 = 40us
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer_initpara);
	
	timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
    timer_interrupt_enable(TIMER3, TIMER_INT_UP);
    timer_enable(TIMER3);
}


void SignalTimerEnable(void)
{
	timer_enable(TIMER2);
}

void SignalTimerDisable(void)
{
	timer_disable(TIMER2);
}

/**
    \brief      configure the TIMER1 interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER1_IRQn, 1, 1);
	nvic_irq_enable(TIMER2_IRQn, 1, 2);
	nvic_irq_enable(TIMER3_IRQn, 1, 3);
	nvic_irq_enable(DMA0_Channel0_IRQn,0,0);
}

/*!
    \brief      this function handles TIMER1 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER1_IRQHandler(void)
{
	static uint32_t count = 0;
	static uint32_t oneSecondCount = 0;
	static uint32_t halfSecondCount = 0;
	static uint32_t led_count = 0;
	static uint8_t led_state = 0;
	
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
				count++;
				if(count >= 1000)
				{
						//Led1Toggle();
						count = 0;
						TempureInterval++;
						if(TempureInterval >= NbInfo.ConfData.TemCollectInterval)
						{
								systemFlag.bit.Run18B20Inter = 1;
								systemFlag.bit.Run18B20Exter = 1;
								systemFlag.bit.RunCj2301 = 1;
								systemFlag.bit.GetChannel3_Status = 1;
								TempureInterval = 0;
						}
				}
				if(NbInfo.NetStatus)
				{
						if(NbInfo.InfoFlags.bit.CheckDataRes || NbInfo.InfoFlags.bit.CheckRegRes)
						{
								NbInfo.NetLedStatus = NET_LED_STATUS_FLASH_200MS;
								halfSecondCount++;
								if(halfSecondCount >= 200)
								{
										halfSecondCount = 0;
								}
						}
						else
						{
								NbInfo.NetLedStatus = NET_LED_STATUS_FLASH_1000MS;
								oneSecondCount++;
								if(oneSecondCount >= 1000)
								{
										oneSecondCount = 0;
								}
						}
				}
				else
				{
						NbInfo.NetLedStatus = NET_LED_STATUS_OFF;
				}
		
				if(NbInfo.AlarmSendOut)//alarm send out, flash led
				{
						NbInfo.DataLedStatus = DATA_LED_STATUS_ABNORMAL_FLASH;
						led_count++;
						if(led_count >= 500)
						{
								led_count = 0;
								led_state = !led_state;
						}
				}
				if(PakTimer.TimerEnable == TIMER_ENABLE_COMM)
				{
						PakTimer.count++;
						if(PakTimer.count >= PAK_TIMEOUT_VAL)
						{
								PakTimer.count = 0;
								PakTimer.TimerEnable = TIMER_DISABLE_COMM;
								PrtocolStr.step = PAK_HEAD;						
						}
				}
				systemFlag.bit.Time1ms = 1;
    }
}

/*!
    \brief      this function handles TIMER2 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER2_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
		SigInfo.Count++;
    }
}

void TIMER3_IRQHandler(void)
{
	if(SET == timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
		if(systemFlag.bit.AdcReady)
		{
			//systemFlag.bit.AdcNeedSample = 1;
			adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
		}
    }
}
