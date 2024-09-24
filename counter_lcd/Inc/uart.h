/*
 * uart.h
 *
 *  Created on: Sep 23, 2024
 *      Author: spam
 */

#ifndef UART_H_
#define UART_H_

#include"stm32f4xx.h"

#define UART						USART3
#define GPIO_UART 					GPIOB
#define GPIO_APB1ENR_CCLK 			18
#define GPIO_AHB1ENR_CCLK			1

#define UART_TX						10
#define UART_RX						11

#define UBRR_9600					0x683
#define UBRR_38400					0x1A1
#define UBRR_115200					0x8B


void inituart(uint32_t bps);
void uartputc(uint8_t ch);
void uartputs(char str[]);
uint8_t uartgetc(void);
void uartgets(char str[]);

#endif /* UART_H_ */







