


/**************************************************/
#include "hw_timer.h"

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
	//nvic_irq_enable(TIMER2_IRQn, 1, 2);
}

/*!
    \brief      this function handles TIMER1 interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER1_IRQHandler(void)
{	
    if(SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP)){
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
				
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
				
    }
}
