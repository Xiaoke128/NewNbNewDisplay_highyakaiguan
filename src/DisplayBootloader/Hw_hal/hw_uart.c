
#include "hw_uart.h"

static uint8_t NbHead = 0;
static uint8_t NbTail = 0;
static uint8_t NbCount = 0;
static uint8_t NbBuff[HOST_BUFFER_SIZE] = {0};

static uint8_t ComunHead = 0;
static uint8_t ComunTail = 0;
static uint8_t CommunCount = 0;
static uint8_t CommunBuff[HOST_BUFFER_SIZE] = {0};


//uart interface init(communicate with NB module)
static void UartNbInit(void)
{
	/* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* connect port to USARTx_Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    
    /* USART configure */
    usart_deinit(NB_UART);
    usart_baudrate_set(NB_UART, 115200);//9600波特率
    usart_word_length_set(NB_UART, USART_WL_8BIT);//八位
    usart_stop_bit_set(NB_UART, USART_STB_1BIT);//1位停止
    usart_parity_config(NB_UART, USART_PM_NONE);//无校验
    usart_hardware_flow_rts_config(NB_UART, USART_RTS_DISABLE);//无rts流控
    usart_hardware_flow_cts_config(NB_UART, USART_CTS_DISABLE);//无cts流控
    usart_receive_config(NB_UART, USART_RECEIVE_ENABLE);//接收配置
    usart_transmit_config(NB_UART, USART_TRANSMIT_ENABLE);//发射配置
	
	//usart_interrupt_enable(NB_UART, USART_INT_TC);
	usart_interrupt_enable(NB_UART, USART_INT_RBNE);
    usart_enable(NB_UART);//使能串口0
	nvic_irq_enable(USART0_IRQn, 1, 3);
}

#if 0
//Host uart init
static void UartHostInit(void)
{
    /* enable USART clock */
    //rcu_periph_clock_enable(RCU_USART1);
    
    /* enable GPIO clock */
    //rcu_periph_clock_enable(RCU_GPIOA);
        
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    /* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    
    /* USART configure */
    usart_deinit(HOST_UART);
    usart_baudrate_set(HOST_UART, 115200);//115200波特率
    usart_word_length_set(HOST_UART, USART_WL_8BIT);//八位
    usart_stop_bit_set(HOST_UART, USART_STB_1BIT);//1位停止
    usart_parity_config(HOST_UART, USART_PM_NONE);//无校验
    usart_hardware_flow_rts_config(HOST_UART, USART_RTS_DISABLE);//无rts流控
    usart_hardware_flow_cts_config(HOST_UART, USART_CTS_DISABLE);//无cts流控
    usart_receive_config(HOST_UART, USART_RECEIVE_ENABLE);//接收配置
    usart_transmit_config(HOST_UART, USART_TRANSMIT_ENABLE);//发射配置
	
	usart_interrupt_enable(HOST_UART, USART_INT_TC);
	usart_interrupt_enable(HOST_UART, USART_INT_RBNE);
    usart_enable(HOST_UART);//使能串口0
	nvic_irq_enable(USART1_IRQn, 1, 3);
}
#endif
static void UartCommuncateInit(void)
{
	    /* connect port to USARTx_Tx */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    /* connect port to USARTx_Rx */
		gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    
    /* USART configure */
    usart_deinit(COMMUN_UART);
    usart_baudrate_set(COMMUN_UART, 115200);//115200波特率
    usart_word_length_set(COMMUN_UART, USART_WL_8BIT);//八位
    usart_stop_bit_set(COMMUN_UART, USART_STB_1BIT);//1位停止
    usart_parity_config(COMMUN_UART, USART_PM_NONE);//无校验
    usart_hardware_flow_rts_config(COMMUN_UART, USART_RTS_DISABLE);//无rts流控
    usart_hardware_flow_cts_config(COMMUN_UART, USART_CTS_DISABLE);//无cts流控
    usart_receive_config(COMMUN_UART, USART_RECEIVE_ENABLE);//接收配置
    usart_transmit_config(COMMUN_UART, USART_TRANSMIT_ENABLE);//发射配置
	
		//usart_interrupt_enable(COMMUN_UART, USART_INT_TC);
		usart_interrupt_enable(COMMUN_UART, USART_INT_RBNE);
    usart_enable(COMMUN_UART);//使能串口0
		nvic_irq_enable(USART2_IRQn, 1, 3);
}

void UartInit(void)
{
	UartNbInit();
	//UartHostInit();
	UartCommuncateInit();
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(COMMUN_UART, (uint8_t)ch);
    while(RESET == usart_flag_get(COMMUN_UART, USART_FLAG_TBE));
    return ch;
}
#if 0
//host uart send data
//parameter:
//			buf: buffer need to send
//			len: length need to send
void HostUartSendBuf(uint8_t *buf, uint8_t len)
{
	uint8_t i = 0;
	for(i = 0; i < len; i++)
	{
		TX_ENABLE;
		usart_data_transmit(HOST_UART, buf[i]);
		while(RESET == usart_flag_get(HOST_UART, USART_FLAG_TBE));
	}
}
#endif
//nb module uart send data
//parameter:
//			buf: buffer need to send
//			len: length need to send
void NbUartSendBuf(uint8_t *buf, uint8_t len)
{
	uint8_t i = 0;
	for(i = 0; i < len; i++)
	{
		usart_data_transmit(NB_UART, buf[i]);
		while(RESET == usart_flag_get(NB_UART, USART_FLAG_TBE));
	}
}


void CommunUartSendBuf(uint8_t *buf, uint8_t len)
{
	uint8_t i = 0;
	for(i = 0; i < len; i++)
	{
		usart_data_transmit(COMMUN_UART, buf[i]);
		while(RESET == usart_flag_get(COMMUN_UART, USART_FLAG_TBE));
	}
}
#if 0
//host receive data, add to a fifo
void HostAdd(uint8_t data)
{
	nvic_irq_disable(USART1_IRQn);
	HostCount++;
	nvic_irq_enable(USART1_IRQn, 1, 3);
	HostBuff[HostTail++] = data;
	if(HostTail == HOST_BUFFER_SIZE)
	{
		HostTail = 0;
	}
}
//check fifo, whether have data not get out
uint8_t HostCheck(void)
{
	if(HostCount > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//get a character from fifo
uint8_t HostGetData(void)
{
	uint8_t data = 0;
	
	if(HostCount > 0)
	{
		data = HostBuff[HostHead++];
		if(HostHead == HOST_BUFFER_SIZE)
		{
			HostHead = 0;
		}
		nvic_irq_disable(USART1_IRQn);
		HostCount--;
		nvic_irq_enable(USART1_IRQn, 1, 3);
	}
	return data;
}
#endif
//nb uart receive data, add to a fifo
void NbAdd(uint8_t data)
{
	//nvic_irq_disable(USART1_IRQn);
	usart_interrupt_disable(NB_UART, USART_INT_RBNE);
	NbCount++;
	//nvic_irq_enable(USART1_IRQn, 1, 3);
	usart_interrupt_enable(NB_UART, USART_INT_RBNE);
	NbBuff[NbTail++] = data;
	if(NbTail == HOST_BUFFER_SIZE)
	{
		NbTail = 0;
	}
}
//check fifo, whether have data not get out
uint8_t NbCheck(void)
{
	if(NbCount > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//get a character from fifo
uint8_t NbGetData(void)
{
	uint8_t data = 0;
	
	if(NbCount > 0)
	{
		data = NbBuff[NbHead++];
		if(NbHead == HOST_BUFFER_SIZE)
		{
			NbHead = 0;
		}
		//nvic_irq_disable(USART1_IRQn);
		usart_interrupt_disable(NB_UART, USART_INT_RBNE);
		NbCount--;
		//nvic_irq_enable(USART1_IRQn, 1, 3);
		usart_interrupt_enable(NB_UART, USART_INT_RBNE);
	}
	return data;
}

void CommunAdd(uint8_t data)
{
	usart_interrupt_disable(COMMUN_UART, USART_INT_RBNE);
	CommunCount++;
	//nvic_irq_enable(USART1_IRQn, 1, 3);
	usart_interrupt_enable(COMMUN_UART, USART_INT_RBNE);
	CommunBuff[ComunTail++] = data;
	if(ComunTail == HOST_BUFFER_SIZE)
	{
		ComunTail = 0;
	}
}

uint8_t CommunCheck(void)
{
	if(CommunCount > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t CommunGetData(void)
{
	uint8_t data = 0;
	
	if(CommunCount > 0)
	{
		data = CommunBuff[ComunHead++];
		if(ComunHead == HOST_BUFFER_SIZE)
		{
			ComunHead = 0;
		}
		//nvic_irq_disable(USART1_IRQn);
		usart_interrupt_disable(COMMUN_UART, USART_INT_RBNE);
		CommunCount--;
		//nvic_irq_enable(USART1_IRQn, 1, 3);
		usart_interrupt_enable(COMMUN_UART, USART_INT_RBNE);
	}
	return data;
}
#if 0
/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
	uint8_t ch = 0;
	
    if(RESET != usart_interrupt_flag_get(HOST_UART, USART_INT_FLAG_RBNE)){
			ch = usart_data_receive(HOST_UART);
			HostAdd(ch);
			usart_interrupt_flag_clear(HOST_UART, USART_INT_FLAG_RBNE);
    }
    if(RESET != usart_interrupt_flag_get(HOST_UART, USART_INT_FLAG_TC)){
			RX_ENABLE;
			usart_interrupt_flag_clear(HOST_UART, USART_INT_FLAG_TC);
    }
}
#endif

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
	uint8_t ch = 0;
	
    if(RESET != usart_interrupt_flag_get(NB_UART, USART_INT_FLAG_RBNE)){
				ch = usart_data_receive(NB_UART);
				NbAdd(ch);
				usart_interrupt_flag_clear(NB_UART, USART_INT_FLAG_RBNE);
    }
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART2_IRQHandler(void)
{
	uint8_t ch = 0;
	
    if(RESET != usart_interrupt_flag_get(COMMUN_UART, USART_INT_FLAG_RBNE)){
				ch = usart_data_receive(COMMUN_UART);
				CommunAdd(ch);
				usart_interrupt_flag_clear(COMMUN_UART, USART_INT_FLAG_RBNE);
    }
}
