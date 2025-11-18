#include "hw_adc.h"
#include "MT29F2G01ABAGDWB.h"
uint16_t AdcSampVal[2] = {0};
uint16_t AdcSampVal1[256] = {0};
uint16_t AdcSampVal2[256] = {0};

uint16_t RunAdcSampVal1[512] = {0};
uint16_t RunAdcSampVal2[512] = {0};
uint16_t RunAdcSampVal3[512] = {0};
uint16_t RunAdcIndex1 = 0;
uint16_t RunAdcIndex2 = 0;
uint16_t RunAdcIndex3 = 0;
uint8_t bufInUseNum = 0;

uint16_t AdcValIndex = 0;

uint8_t WriteIndex = 0;
static void dma_config(void);
void AdcInit(void)
{
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);//108 / 16 = 6.75MHZ
	
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4|GPIO_PIN_5); 
	
	adc_deinit(ADC0);
	/* ADC mode config */
    adc_mode_config(ADC_MODE_FREE); 
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);  
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE); 
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_13POINT5);//1000000 / (1000000 / 6750000 * (239.5 + 12.5)) = 26.785k
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_5, ADC_SAMPLETIME_13POINT5);
	
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
	
	//adc_interrupt_flag_clear(ADC1, ADC_INT_FLAG_EOC);
    //adc_interrupt_enable(ADC1, ADC_INT_EOC);
	
	dma_config();
	/* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
	/* enable ADC interface */
    adc_enable(ADC0);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
	delay_1ms(1);
	systemFlag.bit.AdcReady = 1;
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

static void dma_config(void)
{
	
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&AdcSampVal);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 2U;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
    dma_circulation_enable(DMA0, DMA_CH0);
  
	dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_INT_FLAG_G);
	dma_interrupt_enable(DMA0, DMA_CH0, DMA_INT_FTF);
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}

uint16_t readAdcVal(uint8_t ch)
{
	adc_regular_channel_config(ADC0, 0, ch, ADC_SAMPLETIME_13POINT5);
	
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
	
	/* wait the end of conversion flag */
    while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    /* return regular channel sample value */
    return (adc_regular_data_read(ADC0));
}

void AdcTask(void)
{
	if(systemFlag.bit.AdcNeedSample)
	{
		systemFlag.bit.AdcNeedSample = 0;
		if(AdcValIndex >= 250) {
			AdcValIndex = 249;
			memmove(AdcSampVal1, &AdcSampVal1[1], 248);
			memmove(AdcSampVal2, &AdcSampVal2[1], 248);
		}
		AdcSampVal1[AdcValIndex] = readAdcVal(ADC_CHANNEL_4);
		AdcSampVal2[AdcValIndex] = readAdcVal(ADC_CHANNEL_5);	
		AdcValIndex++;
	}
}

/*!
    \brief      this function handles DMA0_Channel0_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t adcStart = 0;

void DMA0_Channel0_IRQHandler(void)
{
	static uint16_t count = 0;
	uint16_t temp = 0;
    if(dma_interrupt_flag_get(DMA0, DMA_CH0, DMA_INT_FLAG_FTF)){     
        //g_dmacomplete_flag = 1;
        dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_INT_FLAG_G);
		if(!SigInfo.KnifeFlag.bit.KnifeCloseStart && !SigInfo.KnifeFlag.bit.KnifeOpenStart) {
			//if(AdcValIndex >= 256) {
				//AdcValIndex = 255;
				//memmove(AdcSampVal1, &AdcSampVal1[1], 254 * 2);
				//memmove(AdcSampVal2, &AdcSampVal2[1], 254 * 2);
			//}
			AdcSampVal1[adcStart] = AdcSampVal[0];
			AdcSampVal2[adcStart] = AdcSampVal[1];	
			if(adcStart == 255) {
				adcStart = 0;
			}
			else {
				adcStart++;
			}
		}
		else {
			
			if(SigInfo.KnifeFlag.bit.KnifeCloseStart) {
				temp = AdcSampVal[0];
			}
			else {
				temp = AdcSampVal[1];
			}
			if(bufInUseNum == 0) {
				RunAdcSampVal1[RunAdcIndex1++] = temp;
				if(RunAdcIndex1 >= 512) {
					bufInUseNum = 1;
					RunAdcIndex2 = 0;
					RunAdcIndex3 = 0;
					WriteIndex |= 0x01;
				}
			}
			else if(bufInUseNum == 1){
				RunAdcSampVal2[RunAdcIndex2++] = temp;
				if(RunAdcIndex2 >= 512) {
					bufInUseNum = 2;
					RunAdcIndex1 = 0;
					RunAdcIndex3 = 0;
					WriteIndex |= 0x02;
				}
			}
			else if(bufInUseNum == 2) {
				RunAdcSampVal3[RunAdcIndex3++] = temp;
				if(RunAdcIndex3 >= 512) {
					bufInUseNum = 0;
					RunAdcIndex1 = 0;
					RunAdcIndex2 = 0;
					WriteIndex |= 0x04;
				}
			}
			count++;
			if(count >= 1250) {
				count = 0;
				SigInfo.KnifeFlag.bit.KnifeCloseStart = 0;
				SigInfo.KnifeFlag.bit.KnifeOpenStart = 0;
				WriteIndex |= 0x08;
			}
		}
    }
}


void WriteFlashTask(void)
{
	static uint32_t addr = 0;
	static uint8_t blockIndex = 0;
	uint16_t tempBuf[256] = {0};
	
	if(SigInfo.KnifeFlag.bit.NeedWriteCloseBuf) {
		if(StoreInfo.CurrentBlock == 0) {
			EarseBlock(0);
			StoreInfo.CurrentBlock = 1;
			addr = FLASH_BLOCK0_ADDR;
			blockIndex = 0;
		}
		else {
			EarseBlock(1);
			StoreInfo.CurrentBlock = 0;
			addr = FLASH_BLOCK1_ADDR;
			blockIndex = 1;
		}
		StoreInfo.NumberUseBlock++;
		if(StoreInfo.NumberUseBlock > 2) {
			StoreInfo.NumberUseBlock = 2;
		}
		if(adcStart > 0) {
			memcpy(tempBuf, &AdcSampVal1[adcStart - 1], (255 - adcStart) * 2);
			memcpy(&tempBuf[255 - adcStart], AdcSampVal1, adcStart * 2);
		}
		else {
			memcpy(tempBuf, AdcSampVal1, 256 * 2);
		}
		WriteDataToFlash(addr, (uint8_t *)tempBuf, 256 * 2);
		addr += 512;
		SigInfo.KnifeFlag.bit.NeedWriteCloseBuf = 0;
		StoreInfo.BlockStoreBytes[blockIndex] = 512;
	}
	else if(SigInfo.KnifeFlag.bit.NeedWriteOpenBuf) {
		if(StoreInfo.CurrentBlock == 0) {
			EarseBlock(0);
			StoreInfo.CurrentBlock = 1;
			addr = FLASH_BLOCK0_ADDR;
			blockIndex = 0;
		}
		else {
			EarseBlock(1);
			StoreInfo.CurrentBlock = 0;
			addr = FLASH_BLOCK1_ADDR;
			blockIndex = 1;
		}
		StoreInfo.NumberUseBlock++;
		if(StoreInfo.NumberUseBlock > 2) {
			StoreInfo.NumberUseBlock = 2;
		}
		if(adcStart > 0) {
			memcpy(tempBuf, &AdcSampVal2[adcStart - 1], (255 - adcStart) * 2);
			memcpy(&tempBuf[255 - adcStart], AdcSampVal2, adcStart * 2);
		}
		else {
			memcpy(tempBuf, AdcSampVal2, 256 * 2);
		}
		WriteDataToFlash(addr, (uint8_t *)tempBuf, 256 * 2);
		addr += 512;			
		SigInfo.KnifeFlag.bit.NeedWriteOpenBuf = 0;
		StoreInfo.BlockStoreBytes[blockIndex] = 512;
	}
	if(WriteIndex & 0x01) {
		WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal1, 512 * 2);
		addr += 1024;
		//WriteIndex = 0;
		StoreInfo.BlockStoreBytes[blockIndex] += 1024;
		WriteIndex = WriteIndex & 0xFE;
	}
	else if(WriteIndex & 0x02) {
		WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal2, 512 * 2);
		addr += 1024;
		//WriteIndex = 0;
		StoreInfo.BlockStoreBytes[blockIndex] += 1024;
		WriteIndex = WriteIndex & 0xFD;
	}
	else if(WriteIndex & 0x04) {
		WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal3, 512 * 2);
		addr += 1024;
		//WriteIndex = 0;
		StoreInfo.BlockStoreBytes[blockIndex] += 1024;
		WriteIndex = WriteIndex & 0xFB;
	}
	else if(WriteIndex & 0x08) {
		if(bufInUseNum == 1) {
			WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal2, RunAdcIndex2 * 2);
			StoreInfo.BlockStoreBytes[blockIndex] += RunAdcIndex2 * 2;
		}
		else if(bufInUseNum == 0){
			WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal1, RunAdcIndex1 * 2);
			StoreInfo.BlockStoreBytes[blockIndex] += RunAdcIndex1 * 2;
		}
		else if(bufInUseNum == 2) {
			WriteDataToFlash(addr, (uint8_t *)RunAdcSampVal3, RunAdcIndex3 * 2);
			StoreInfo.BlockStoreBytes[blockIndex] += RunAdcIndex3 * 2;
		}
		//WriteIndex = 0;
		RunAdcIndex1 = 0;
		RunAdcIndex2 = 0;
		RunAdcIndex3 = 0;
		StoreInfo.CrcVal = CheckCRC((uint8_t *)&StoreInfo, sizeof(StoreInfoStr) - 4);
		WriteStoreInfo();
		WriteIndex = WriteIndex & 0xF7;
	}
	
}

