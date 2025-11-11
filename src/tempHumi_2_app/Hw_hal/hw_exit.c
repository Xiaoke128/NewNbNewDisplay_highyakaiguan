#include "hw_exit.h"


//exit interruprt init, channel 1 and 2 falling edge, channel 3 falling and rising
void ExitInit(void)
{
	//rcu_periph_clock_enable(RCU_AF);

#if 1
	//channel 1
    gpio_init(EXIT_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, EXIT1_PIN);
    /* enable and set key EXTI interrupt priority */
    //nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, EXIT1_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(EXIT1_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXIT1_SOURCE_CHAN);	
	
	//channel 2
	gpio_init(EXIT_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, EXIT2_PIN);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, EXIT2_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(EXIT2_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXIT2_SOURCE_CHAN);	
	
	//channel 3
	gpio_init(EXIT_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, EXIT3_PIN);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, EXIT3_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(EXIT3_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(EXIT3_SOURCE_CHAN);	

	//channel 4
	gpio_init(EXIT_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, EXIT4_PIN);
#if 1
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, EXIT4_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(EXIT4_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXIT4_SOURCE_CHAN);	
#endif
	
	nvic_irq_enable(EXTI10_15_IRQn, 1U, 0U);
#else
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_0);
    /* configure key EXTI line */
    exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(EXTI_0);	
	
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOA, GPIO_PIN_SOURCE_1);
    /* configure key EXTI line */
    exti_init(EXTI_1, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    exti_interrupt_flag_clear(EXTI_1);
	
	/* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI0_IRQn, 1U, 0U);
	nvic_irq_enable(EXTI1_IRQn, 1U, 2U);
#endif
}

//get channel 1 ~ 3 level
void GetExitPinVal(void)
{
	static uint16_t count = 0, checkCount1 = 0, checkCount2 = 0;
	FlagStatus t1, t2, t3, t4, m1, m2;
	static FlagStatus old_t1 = RESET, old_t2 = RESET;
	
	t1 = GET_EXIT_CHANNEL1_VAL;
	t2 = GET_EXIT_CHANNEL2_VAL;
	t3 = GET_EXIT_CHANNEL3_VAL;
	t4 = GET_EXIT_CHANNEL4_VAL;
	//m1 = GET_MODE1_VAL;
	//m2 = GET_MODE2_VAL;
	
	//if(m1 && m2)
	//{
		//M1 AND M2 all on
		//SigInfo.KnifeFlag.bit.ModeCheck = 1;
	//}
	//else
	//{
		//SigInfo.KnifeFlag.bit.ModeCheck = 0;
	//}
	if(t1 == old_t1)
	{
		if(t1 == SET)
		{
			checkCount1++;
			if(checkCount1 >= 10)
			{
				//high for 10ms, consider t1 in high level
				checkCount1 = 10;
				systemFlag.bit.Channel1_High = 1;
			}
		}
		else
		{
			checkCount1 = 0;
			systemFlag.bit.Channel1_High = 0;
		}
	}
	else
	{
		old_t1 = t1;
		checkCount1 = 0;
		systemFlag.bit.Channel1_High = 0;
	}
	if(t2 == old_t2)
	{
		if(t2 == SET)
		{
			checkCount2++;
			if(checkCount2 >= 10)
			{
				//high for 10ms, consider t1 in high level
				checkCount2 = 10;
				systemFlag.bit.Channel2_High = 1;
			}
		}
		else
		{
			checkCount2 = 0;
			systemFlag.bit.Channel2_High = 0;
		}
	}
	else
	{
		old_t2 = t2;
		checkCount2 = 0;
		systemFlag.bit.Channel2_High = 0;
	}
	count++;
	if(count >= 5000)//5 seconds prinf once
	{
		count = 0;
		if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
		{
				TX_ENABLE;
				printf("Exit channel val: T1->%d, T2->%d, T3->%d, T4->%d \r\n", t1, t2, t3, t4);
				TX_ENABLE;
				printf("Mode val: %d%d \r\n", m1, m2);
		}
	}
}

void GetChannel3_Status(void)
{
	if(systemFlag.bit.GetChannel3_Status)
	{
		systemFlag.bit.GetChannel3_Status = 0;
		if(StoreConf.Ch3_Set == 0) {
			NbInfo.Channel3_Status = GET_EXIT_CHANNEL3_VAL;
		}
		else {
			NbInfo.Channel3_Status = !GET_EXIT_CHANNEL3_VAL;
		}
	}
	if(NbInfo.CloseDelayFlag)
	{
		NbInfo.CloseDelayCount++;
		if(NbInfo.CloseDelayCount >= 1000)
		{
			NbInfo.CloseDelayCount = 0;
			NbInfo.CloseDelayFlag = 0;
		}
	}
	if(NbInfo.OpenDelayFlag)
	{
		NbInfo.OpenDelayCount++;
		if(NbInfo.OpenDelayCount >= 1000)
		{
			NbInfo.OpenDelayCount = 0;
			NbInfo.OpenDelayFlag = 0;
		}
	}
}

void SignalCheck(void)
{
	static FlagStatus ch1_val = RESET, ch1_val_old = RESET, ch2_val = RESET, ch2_val_old = RESET, ch3_val = RESET, ch3_val_old = RESET;
	static uint8_t ch1_Count = 0, ch2_Count = 0, ch3_Count = 0;
	static uint8_t ch1_float = 0, ch2_float = 0, ch3_float = 0;
	static uint8_t ch1_flag = 0, ch2_flag = 0, ch3_flag = 0;
	static uint16_t error1_count = 0, error2_count = 0;
	float ff;
	
	if(SigInfo.KnifeFlag.bit.KnifeClose)
	{
		NbInfo.Stype = SIGNAL_CLOSE;
		error1_count++;
		if(error1_count >= 1000)
		{
			NbInfo.AlarmFlags.bit.CloseAlarm = 1;
			NbInfo.AlarmSendOut = 0;
			SigInfo.KnifeFlag.val = 0;
			SigInfo.KnifeCloseTemp = 0;
			SignalTimerDisable();			
			NbInfo.Stime = 10000;//1 SECOND
			NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
			//NbInfo.InfoFlags.bit.OpenCloseActSend = 1;
			StoreInfoAdd();
			if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
			{
					TX_ENABLE;
					printf("Knife close no feed back more than 1s!\r\n");
			}
			//RX_ENABLE;
		}
		if(ch1_flag)
		{
			ch1_float++;
			ch1_val = gpio_input_bit_get(EXIT_PORT, EXIT1_PIN);
			if(ch1_val == ch1_val_old)
			{
				ch1_Count++;
			}
			else
			{
				ch1_val_old = ch1_val;
				ch1_Count = 0;
			}
			if(ch1_Count >= SIGNAL_MIN_TRIGGER_TIME)
			{
				ch1_flag = 0;
				ch1_Count = 0;
				if(ch1_val != RESET)
				{
					SigInfo.KnifeFlag.val = 0;
					SignalTimerDisable();	
					NbInfo.Stime = 0;
					NbInfo.Stype = SIGNAL_NO;
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("Less than 15ms for channel trigger.\r\n");
					}
				}
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
						TX_ENABLE;
						printf("Knife close shake time is: %dms\r\n", ch1_float - ch1_Count);
				}
				//RX_ENABLE;
			}
		}
	}
	else
	{
		error1_count = 0;
		ch1_flag = 1;
		ch1_float = 0;
	}
	if(SigInfo.KnifeFlag.bit.KnifeOpen)
	{
		NbInfo.Stype = SIGNAL_OPEN;
		error2_count++;
		if(error2_count >= 1000)
		{
			NbInfo.AlarmFlags.bit.OpenAlarm = 1;
			NbInfo.AlarmSendOut = 0;
			SigInfo.KnifeFlag.val = 0;
			SignalTimerDisable();			
			NbInfo.Stime = 10000;//1 SECOND
			NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
			//NbInfo.InfoFlags.bit.OpenCloseActSend = 1;
			StoreInfoAdd();
			if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
			{
					TX_ENABLE;
					printf("Knife open no feed back more than 1s!\r\n");
			}
			//RX_ENABLE;
		}
		if(ch2_flag)
		{
			ch2_float++;
			ch2_val = GET_EXIT_CHANNEL2_VAL;
			if(ch2_val == ch2_val_old)
			{
				ch2_Count++;
			}
			else
			{
				ch2_val_old = ch2_val;
				ch2_Count = 0;
			}
			if(ch2_Count >= SIGNAL_MIN_TRIGGER_TIME)
			{
				ch2_flag = 0;
				ch2_Count = 0;
				if(ch2_val != RESET)
				{
					SigInfo.KnifeFlag.val = 0;
					SignalTimerDisable();	
					NbInfo.Stime = 0;
					NbInfo.Stype = SIGNAL_NO;
					if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
					{
							TX_ENABLE;
							printf("Less than 15ms for channel trigger.\r\n");
					}
				}
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
						TX_ENABLE;
						printf("Knife open shake time is: %dms\r\n", ch2_float - ch2_Count);
				}
				//RX_ENABLE;
			}
		}
	}
	else
	{
		error2_count = 0;
		ch2_flag = 1;
		ch2_float = 0;
	}
	if(SigInfo.KnifeFlag.bit.KnifeFeedBack)//feed back flag
	{
		if(ch3_flag)
		{
			ch3_float++;
			//if(!SigInfo.KnifeFlag.bit.ModeCheck)//mode not all on, check T3
			//{
				ch3_val = GET_EXIT_CHANNEL3_VAL;
			//}
			//else//mode all on, check T4
			//{
				//ch3_val = GET_EXIT_CHANNEL4_VAL;
			//}
			if(ch3_val == ch3_val_old)
			{
				ch3_Count++;
			}
			else
			{
				ch3_val_old = ch3_val;
				ch3_Count = 0;
			}
			if(ch3_Count >= 10)
			{
				ch3_Count = 0;
				SigInfo.KnifeFlag.val = 0;
				
				ch3_flag = 0;
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
						TX_ENABLE;
						printf("Feed back shake time is: %dms\r\n", ch3_float - ch3_Count);
				}
				ff = (float)SigInfo.UseCount / 10.0;
				if(systemFlag.bit.ModbusOrNbiot == COMMUNICATE_NBIOT)
				{
						TX_ENABLE;
						printf("Knife close/open time is: %.1fms", ff);
				}
				NbInfo.Stime = (uint32_t)(ff * 10);
				//NbInfo.InfoFlags.bit.OpenCloseActSend = 1;
				if(NbInfo.Stime > ACTION_MIN_TIME_MS)//有效动作信号
				{
					if(NbInfo.Stype == SIGNAL_CLOSE)
					{
						NbInfo.CloseDelayFlag = 1;
						if(NbInfo.Stime > StoreConf.ServerConf.CloseAlramTime)
						{
							NbInfo.AlarmFlags.bit.CloseAlarm = 1;
							NbInfo.AlarmSendOut = 0;
							NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
						}
						else
						{
							NbInfo.AlarmFlags.bit.CloseAlarm = 0;
							NbInfo.AlarmSendOut = 0;
						}
						if(GET_EXIT_CHANNEL2_VAL)//分闸信号一直有
						{
								SigInfo.KnifeFlag.bit.KnifeOpen = 1;
								SigInfo.Count = 10;
								SignalTimerEnable();
						}
						SigInfo.KnifeCloseTemp = 0;
					}
					else if(NbInfo.Stype == SIGNAL_OPEN)
					{
						NbInfo.OpenDelayFlag = 1;
						if(NbInfo.Stime > StoreConf.ServerConf.OpenAlarmTime)
						{
							NbInfo.AlarmFlags.bit.OpenAlarm = 1;
							NbInfo.AlarmSendOut = 0;
							NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
						}
						else
						{
							NbInfo.AlarmFlags.bit.OpenAlarm = 0;
							NbInfo.AlarmSendOut = 0;
						}
						if(SigInfo.KnifeCloseTemp)
						{
								SigInfo.KnifeFlag.bit.KnifeClose = 1;
								SigInfo.KnifeCloseTemp = 0;
								SigInfo.Count = 0;
								SignalTimerEnable();
						}
					}
					StoreInfoAdd();
				}
				else//无效信号
				{
					NbInfo.Stime = 0;
					NbInfo.Stype = SIGNAL_NO;
					printf("Interference signal, no send!");
				}				
				SigInfo.UseCount = 0;				
			}
		}
	}
	else
	{
		ch3_flag = 1;
		ch3_float = 0;
	}
}

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI10_15_IRQHandler(void)
{
	if(RESET != exti_interrupt_flag_get(EXIT1_SOURCE_CHAN)){//channel 1
		exti_interrupt_flag_clear(EXIT1_SOURCE_CHAN);
		if(!SigInfo.KnifeFlag.bit.KnifeClose && systemFlag.bit.Channel1_High && NbInfo.Channel3_Status && !NbInfo.CloseDelayFlag)
		{
			if(SigInfo.KnifeFlag.bit.KnifeOpen)//如果在检测分闸的下一瞬间检测到合闸信号，先当合闸信号处理，处理完检测分闸信号
			{
					SigInfo.KnifeFlag.bit.KnifeOpen = 0;
			}
			SigInfo.KnifeFlag.bit.KnifeClose = 1;
			SigInfo.KnifeFlag.bit.NeedWriteCloseBuf = 1;
			SigInfo.KnifeFlag.bit.KnifeCloseStart = 1;
			SigInfo.KnifeCloseTemp = 1;
			SigInfo.Count = 0;
			SignalTimerEnable();
			//printf("Channel 1 start.\r\n");
		}
    }
    if(RESET != exti_interrupt_flag_get(EXIT2_SOURCE_CHAN)){//channel 2
		exti_interrupt_flag_clear(EXIT2_SOURCE_CHAN);
		if(!SigInfo.KnifeFlag.bit.KnifeOpen && systemFlag.bit.Channel2_High && !NbInfo.Channel3_Status && !NbInfo.OpenDelayFlag)
		{
			if(SigInfo.KnifeFlag.bit.KnifeClose)
			{
					SigInfo.KnifeFlag.bit.KnifeClose = 0;
			}
			SigInfo.KnifeFlag.bit.KnifeOpen = 1;
			SigInfo.KnifeFlag.bit.NeedWriteOpenBuf = 1;
			SigInfo.KnifeFlag.bit.KnifeOpenStart = 1;
			SigInfo.Count = 0;
			SignalTimerEnable();
		}
    }
	if(RESET != exti_interrupt_flag_get(EXIT3_SOURCE_CHAN)){//channel 3
		exti_interrupt_flag_clear(EXIT3_SOURCE_CHAN);
		if((!SigInfo.KnifeFlag.bit.KnifeFeedBack) && SigInfo.KnifeFlag.val)
		{
			SigInfo.KnifeFlag.bit.ModeCheck = 0;//check t3
			SigInfo.KnifeFlag.bit.KnifeFeedBack = 1;
			SignalTimerDisable();
			SigInfo.UseCount = SigInfo.Count;
		}
    }
#if 1
	if(RESET != exti_interrupt_flag_get(EXIT4_SOURCE_CHAN)){//channel 4
		exti_interrupt_flag_clear(EXIT4_SOURCE_CHAN);
		NbInfo.Chanel4_AlarmFlag = 1;
		NbInfo.DataLedStatus = DATA_LEN_STATUS_ALARM;
		//NbInfo.InfoFlags.bit.Chanel4_AlarmSend = 1;
#if 0
		if((!SigInfo.KnifeFlag.bit.KnifeFeedBack) && SigInfo.KnifeFlag.val)
		{
			SigInfo.KnifeFlag.bit.ModeCheck = 1;//check t4
			SigInfo.KnifeFlag.bit.KnifeFeedBack = 1;
			SignalTimerDisable();
		}
#endif
    }
#endif
}
