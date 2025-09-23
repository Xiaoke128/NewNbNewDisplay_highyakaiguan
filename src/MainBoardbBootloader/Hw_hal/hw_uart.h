#ifndef HW_UART_H
#define HW_UART_H

#include <stdio.h>
#include "gd32f10x.h"
#include "Init.h"

#define HOST_UART		USART1
#define NB_UART			USART0
#define COMMUN_UART	USART2

#define HOST_BUFFER_SIZE		255

void UartInit(void);

void HostUartSendBuf(uint8_t *buf, uint8_t len);
void NbUartSendBuf(uint8_t *buf, uint8_t len);
void CommunUartSendBuf(uint8_t *buf, uint8_t len);
void HostAdd(uint8_t data);
uint8_t HostCheck(void);
uint8_t HostGetData(void);

void NbAdd(uint8_t data);
uint8_t NbGetData(void);
uint8_t NbCheck(void);

void CommunAdd(uint8_t data);
uint8_t CommunCheck(void);
uint8_t CommunGetData(void);

void USART1_IRQHandler(void);
void USART0_IRQHandler(void);
void USART2_IRQHandler(void);
#endif
