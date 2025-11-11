#ifndef HW_HAL_H
#define HW_HAL_H
#include "gd32f10x_timer.h"
#include "gd32f10x_rcu.h"
#include "Init.h"

void AdcTimerInit(void);
void Timer_1ms_Init(void);
void SignalTimerInit(void);
void SignalTimerEnable(void);
void SignalTimerDisable(void);
void nvic_config(void);

void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);
void TIMER3_IRQHandler(void);
#endif
