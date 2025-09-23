#ifndef HW_UART_H
#define HW_UART_H

#include <stdio.h>
#include "gd32f10x.h"
#include "Init.h"

#define NB_UART			USART0
#define COMMUN_UART	USART2

#define HOST_BUFFER_SIZE		255

void UartInit(void);

void NbUartSendBuf(uint8_t *buf, uint8_t len);
void CommunUartSendBuf(uint8_t *buf, uint8_t len);

void NbAdd(uint8_t data);
uint8_t NbGetData(void);
uint8_t NbCheck(void);

void CommunAdd(uint8_t data);
uint8_t CommunCheck(void);
uint8_t CommunGetData(void);

void USART0_IRQHandler(void);
void USART2_IRQHandler(void);
#endif
