#include "hw_adc.h"

void AdcInit(void)
{
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV16);//108 / 16 = 6.75MHZ
	
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4|GPIO_PIN_5); 
	
	adc_deinit(ADC1);
	/* ADC mode config */
    adc_mode_config(ADC_MODE_FREE); 
	adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);  
	adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC1, ADC_REGULAR_CHANNEL, 2);
	adc_regular_channel_config(ADC1, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_239POINT5);//1000000 / (1000000 / 6750000 * (239.5 + 12.5)) = 26.785k
    adc_regular_channel_config(ADC1, 1, ADC_CHANNEL_5, ADC_SAMPLETIME_239POINT5);
	
	adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
	adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
	
	//adc_interrupt_flag_clear(ADC1, ADC_INT_FLAG_EOC);
    //adc_interrupt_enable(ADC1, ADC_INT_EOC);
	
	/* enable ADC interface */
    adc_enable(ADC1);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
	delay_1ms(1);
	adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL);
}

/*!
    \brief      this function handles ADC0 and ADC1 interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ADC0_1_IRQHandler(void)
{
    /* clear the ADC flag */
    adc_interrupt_flag_clear(ADC1, ADC_INT_FLAG_EOC);
    /* read ADC inserted group data register */
    adc_regular_data_read(ADC1, ADC_INSERTED_CHANNEL_4);
    adc_regular_data_read(ADC1, ADC_INSERTED_CHANNEL_5);
}