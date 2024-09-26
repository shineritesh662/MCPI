/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 20, 2024
*/

#include "lcd.h"

void LcdInit(void) {
	// GPIO initialization
	// enable gpio clock
	RCC->AHB1ENR |= BV(GPIO_LCD_CLKEN);
	// set all lcd pins as output pins -- MODER
	GPIO_LCD->MODER &= ~(BV(LCD_DB4*2+1) | BV(LCD_DB5*2+1) | BV(LCD_DB6*2+1) | BV(LCD_DB7*2+1) | BV(LCD_RS*2+1) | BV(LCD_RW*2+1) | BV(LCD_EN*2+1));
	GPIO_LCD->MODER |= BV(LCD_DB4*2) | BV(LCD_DB5*2) | BV(LCD_DB6*2) | BV(LCD_DB7*2) | BV(LCD_RS*2) | BV(LCD_RW*2) | BV(LCD_EN*2);
	// set pin output type -- pushpull
	GPIO_LCD->OTYPER &= ~(BV(LCD_DB4) | BV(LCD_DB5) | BV(LCD_DB6) | BV(LCD_DB7) | BV(LCD_RS) | BV(LCD_RW) | BV(LCD_EN));
	// set pin speed to low
	//GPIO_LCD->OSPEEDR &= ~(BV(LCD_DB4*2+1) | BV(LCD_DB5*2+1) | BV(LCD_DB6*2+1) | BV(LCD_DB7*2+1) | BV(LCD_RS*2+1) | BV(LCD_RW*2+1) | BV(LCD_EN*2+1));
	//GPIO_LCD->OSPEEDR &= ~(BV(LCD_DB4*2) | BV(LCD_DB5*2) | BV(LCD_DB6*2) | BV(LCD_DB7*2) | BV(LCD_RS*2) | BV(LCD_RW*2) | BV(LCD_EN*2));
	// disbale pin pullup and pulldown resistors
	//GPIO_LCD->PUPDR &= ~(BV(LCD_DB4*2+1) | BV(LCD_DB5*2+1) | BV(LCD_DB6*2+1) | BV(LCD_DB7*2+1) | BV(LCD_RS*2+1) | BV(LCD_RW*2+1) | BV(LCD_EN*2+1));
	//GPIO_LCD->PUPDR &= ~(BV(LCD_DB4*2) | BV(LCD_DB5*2) | BV(LCD_DB6*2) | BV(LCD_DB7*2) | BV(LCD_RS*2) | BV(LCD_RW*2) | BV(LCD_EN*2));

	//clear all data and control pins
	GPIO_LCD->ODR &= ~(BV(LCD_DB7) | BV(LCD_DB6) | BV(LCD_DB5) | BV(LCD_DB4) | BV(LCD_RS) | BV(LCD_RW) | BV(LCD_EN));

	// LCD initialization
	DelayMs(200);
	LcdWriteByte(LCD_CMD, LCD_FN_4BIT_2LINE_5X8);
	LcdWriteByte(LCD_CMD, LCD_DISPON_CUROFF);
	LcdWriteByte(LCD_CMD, LCD_ENTRY_INC_NOSHIFT);
	LcdWriteByte(LCD_CMD, LCD_CLEAR);

	DelayMs(200);
}

void LcdWrite4Bit(uint8_t rs, uint8_t val) {
	// change rs to 0 for instruction OR rs to 1 for data
	if(rs == LCD_CMD)
		GPIO_LCD->ODR &= ~BV(LCD_RS);
	else
		GPIO_LCD->ODR |= BV(LCD_RS);
	// RW=0
	GPIO_LCD->ODR &= ~BV(LCD_RW);
	// write data on DB7 to DB4 pins
	// step1: clear previously written data = 0000
	GPIO_LCD->ODR &= ~(BV(LCD_DB7) | BV(LCD_DB6) | BV(LCD_DB5) | BV(LCD_DB4));
	// step2: write new data on DB7 to DB4 (shift is not required if DB4-DB7 mapped to pins 0-3)
	GPIO_LCD->ODR |= (uint32_t)val << LCD_DB4;
	// EN=1
	GPIO_LCD->ODR |= BV(LCD_EN);
	DelayMs(1);
	// EN=0
	GPIO_LCD->ODR &= ~BV(LCD_EN);
}

void LcdBusyWait(void) {
	// RS=0
	GPIO_LCD->ODR &= ~BV(LCD_RS);
	// RW=1, EN=1
	GPIO_LCD->ODR |= BV(LCD_RW) | BV(LCD_EN);
	// Change mode of LCD_D7 pin to Input
	GPIO_LCD->MODER &= ~(BV(LCD_DB7*2) | BV(LCD_DB7*2+1));
	// Read using IDR
	while((GPIO_LCD->IDR & BV(LCD_DB7)) != 0)
		;	// if BF=1, then Repeat Read.
	// EN=0
	GPIO_LCD->ODR &= ~(BV(LCD_EN) | BV(LCD_RW));
	// Change mode of LCD_D7 pin to Output
	GPIO_LCD->MODER |= BV(LCD_DB7*2);
}

void LcdWriteByte(uint8_t rs, uint8_t val) {
	uint8_t high = val >> 4, low = val & 0x0F;
	LcdWrite4Bit(rs, high); // write higher nibble
	LcdWrite4Bit(rs, low); 	// write lower nibble
	LcdBusyWait(); 			// wait for bust flag
	DelayMs(3);
}

void LcdPuts(uint8_t line, char str[]) {
	// set line address
	LcdWriteByte(LCD_CMD, line);
	// write chars from str
	for(int i=0; str[i]!='\0'; i++)
		LcdWriteByte(LCD_DATA, str[i]);
}


