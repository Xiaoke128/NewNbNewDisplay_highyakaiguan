#include "hw_exit.h"

static ButtonFlag Bflag = {0};
static KeyVal keyVal = KEY_NONE;
//exit interruprt init, channel 1 and 2 falling edge, channel 3 falling and rising
void ButtonInit(void)
{
		//Button up
    gpio_init(BUTTON_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, BUTTON_UP_PIN);
    /* enable and set key EXTI interrupt priority */
    //nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, BUTTON_UP_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(BUTTON_UP_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(BUTTON_UP_SOURCE_CHAN);	
	
		//Button down
		gpio_init(BUTTON_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, BUTTON_DOWN_PIN);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, BUTTON_DOWN_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(BUTTON_DOWN_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(BUTTON_DOWN_SOURCE_CHAN);	
	
		//Button back
		gpio_init(BUTTON_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, BUTTON_BACK_PIN);
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, BUTTON_BACK_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(BUTTON_BACK_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(BUTTON_BACK_SOURCE_CHAN);	

		//Button enter
		gpio_init(BUTTON_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, BUTTON_ENTER_PIN);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, BUTTON_ENTER_SOURCE_PIN);
    /* configure key EXTI line */
    exti_init(BUTTON_ENTER_SOURCE_CHAN, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(BUTTON_ENTER_SOURCE_CHAN);	

	
		nvic_irq_enable(EXTI10_15_IRQn, 1U, 0U);
}

KeyVal GetButtonVal(void)
{
		KeyVal tempKey = keyVal;
		
		keyVal = KEY_NONE;
		return tempKey;
}

static void KeyUpScan(void)
{
		static bool PressFlag = FALSE, UpFlag = FALSE;
		static uint16_t count = 0, pressTime = 0;
	
		if(Bflag.bit.keyUpInterrupt)
		{
				if(!PressFlag)
				{
						if(GET_BUTTON_UP_VAL)
						{
								count = 0;
						}
						else
						{
								count++;
						}
						if(count >= 10)
						{
								count = 0;
								pressTime = 10;
								PressFlag = TRUE;
						}
				}
				else
				{
						if(!UpFlag)
						{
								pressTime++;
								if(GET_BUTTON_UP_VAL)
								{
										count++;
								}
								else
								{
										count = 0;
								}
								if(count >= 10)
								{
										count = 0;
										UpFlag = TRUE;
								}
						}
						else
						{
								if(pressTime >= 1500)//long press
								{
										keyVal = KEY_UP_LONG;
								}
								else//short press
								{
										keyVal = KEY_UP_CLICK;
								}
								Bflag.bit.keyUpInterrupt = 0;
								pressTime = 0;
								count = 0;
								PressFlag = FALSE;
								UpFlag = FALSE;
						}
				}
		}
}

static void KeyDownScan(void)
{
		static bool PressFlag = FALSE, UpFlag = FALSE;
		static uint16_t count = 0, pressTime = 0;
	
		if(Bflag.bit.keyDownInterrupt)
		{
				if(!PressFlag)
				{
						if(GET_BUTTON_DOWN_VAL)
						{
								count = 0;
						}
						else
						{
								count++;
						}
						if(count >= 10)
						{
								count = 0;
								pressTime = 10;
								PressFlag = TRUE;
						}
				}
				else
				{
						if(!UpFlag)
						{
								pressTime++;
								if(GET_BUTTON_DOWN_VAL)
								{
										count++;
								}
								else
								{
										count = 0;
								}
								if(count >= 10)
								{
										count = 0;
										UpFlag = TRUE;
								}
						}
						else
						{
								if(pressTime >= 1500)//long press
								{
										keyVal = KEY_DOWN_LONG;
								}
								else//short press
								{
										keyVal = KEY_DOWN_CLICK;
								}
								Bflag.bit.keyDownInterrupt = 0;
								pressTime = 0;
								count = 0;
								PressFlag = FALSE;
								UpFlag = FALSE;
						}
				}
		}
}

static void KeyBackScan(void)
{
		static bool PressFlag = FALSE, UpFlag = FALSE;
		static uint16_t count = 0, pressTime = 0;
	
		if(Bflag.bit.keyBackInterrupt)
		{
				if(!PressFlag)
				{
						if(GET_BUTTON_BACK_VAL)
						{
								count = 0;
						}
						else
						{
								count++;
						}
						if(count >= 10)
						{
								count = 0;
								pressTime = 10;
								PressFlag = TRUE;
						}
				}
				else
				{
						if(!UpFlag)
						{
								pressTime++;
								if(GET_BUTTON_BACK_VAL)
								{
										count++;
								}
								else
								{
										count = 0;
								}
								if(count >= 10)
								{
										count = 0;
										UpFlag = TRUE;
								}
						}
						else
						{
								if(pressTime >= 1500)//long press
								{
										keyVal = KEY_BACK_LONG;
								}
								else//short press
								{
										keyVal = KEY_BACK_CLICK;
								}
								Bflag.bit.keyBackInterrupt = 0;
								pressTime = 0;
								count = 0;
								PressFlag = FALSE;
								UpFlag = FALSE;
						}
				}
		}
}

static void KeyEnterScan(void)
{
		static bool PressFlag = FALSE, UpFlag = FALSE;
		static uint16_t count = 0, pressTime = 0;
	
		if(Bflag.bit.keyEnterInterrupt)
		{
				if(!PressFlag)
				{
						if(GET_BUTTON_ENTER_VAL)
						{
								count = 0;
						}
						else
						{
								count++;
						}
						if(count >= 10)
						{
								count = 0;
								pressTime = 10;
								PressFlag = TRUE;
						}
				}
				else
				{
						if(!UpFlag)
						{
								pressTime++;
								if(GET_BUTTON_ENTER_VAL)
								{
										count++;
								}
								else
								{
										count = 0;
								}
								if(count >= 10)
								{
										count = 0;
										UpFlag = TRUE;
								}
						}
						else
						{
								if(pressTime >= 1500)//long press
								{
										keyVal = KEY_ENETR_LONG;
								}
								else//short press
								{
										keyVal = KEY_ENTER_CLICK;
								}
								Bflag.bit.keyEnterInterrupt = 0;
								pressTime = 0;
								count = 0;
								PressFlag = FALSE;
								UpFlag = FALSE;
						}
				}
		}
}

void ButtonScan(void)
{
		static uint32_t count = 0;
	
		KeyUpScan();
		KeyDownScan();
		KeyBackScan();
		KeyEnterScan();
		if(keyVal == KEY_NONE)
		{
				count++;
				if(count >= KEY_NONE_FOR_5_MINUTES)//KEY_NONE_FOR_5_MINUTES
				{
						count = 0;
						if(GUI_Func != MechProperitesFun)
						{
								systemFlag.bit.NoActJump = 1;
								GUI_Func = MechProperitesFun;
						}
				}
		}
		else
		{
				count = 0;
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
		if(RESET != exti_interrupt_flag_get(BUTTON_UP_SOURCE_CHAN)){//channel 1
				exti_interrupt_flag_clear(BUTTON_UP_SOURCE_CHAN);
				if(!Bflag.bit.keyUpInterrupt)
				{
						Bflag.bit.keyUpInterrupt = 1;
				}
    }
    if(RESET != exti_interrupt_flag_get(BUTTON_DOWN_SOURCE_CHAN)){//channel 2
				exti_interrupt_flag_clear(BUTTON_DOWN_SOURCE_CHAN);
				if(!Bflag.bit.keyDownInterrupt)
				{
						Bflag.bit.keyDownInterrupt = 1;
				}
    }
		if(RESET != exti_interrupt_flag_get(BUTTON_BACK_SOURCE_CHAN)){//channel 3
				exti_interrupt_flag_clear(BUTTON_BACK_SOURCE_CHAN);
				if(!Bflag.bit.keyBackInterrupt)
				{
						Bflag.bit.keyBackInterrupt = 1;
				}
    }
		if(RESET != exti_interrupt_flag_get(BUTTON_ENTER_SOURCE_CHAN)){//channel 4
				exti_interrupt_flag_clear(BUTTON_ENTER_SOURCE_CHAN);
				if(!Bflag.bit.keyEnterInterrupt)
				{
						Bflag.bit.keyEnterInterrupt = 1;
				}
    }
}
