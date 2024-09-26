/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 20, 2024
*/

#ifndef LCD_H_
#define LCD_H_

#include "stm32f4xx.h"

// circuit connections
#define GPIO_LCD_CLKEN	3
#define GPIO_LCD		GPIOD
#define LCD_DB4			0
#define LCD_DB5			1
#define LCD_DB6			2
#define LCD_DB7			3
#define LCD_RS			5
#define LCD_RW			6
#define LCD_EN			7

// lcd defines
#define LCD_CLEAR				0x01
#define LCD_FN_4BIT_2LINE_5X8	0x28
#define LCD_ENTRY_INC_NOSHIFT	0x06
#define LCD_DISPON_CUROFF		0x0C
#define LCD_LINE1	0x80
#define LCD_LINE2	0xC0

#define LCD_CMD		0
#define LCD_DATA	1

// lcd functions
void LcdInit(void);
void LcdWrite4Bit(uint8_t rs, uint8_t val);
void LcdBusyWait(void);
void LcdWriteByte(uint8_t rs, uint8_t val);
void LcdPuts(uint8_t line, char str[]);

#endif /* LCD_H_ */
