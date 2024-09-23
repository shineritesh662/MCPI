/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 19, 2024
*/

#include "switch.h"

void SwitchInit(void) {
	// enable GPIOA clock -- RCC AHB1ENR[GPIO_CLKEN] = 1
	RCC->AHB1ENR |= BV(GPIO_SWITCH_CLKEN);
	// config GPIOA.pin as input -- MODER[2*pin+1:2*pin] - 00
	GPIO_SWITCH->MODER &= ~(BV(SWITCH_PIN*2) | BV(SWITCH_PIN*2+1));
	// no need to configure OSPEEDR and OTYPER
	// config GPIOA.pin with no pullup & pulldown resistor PUPDR[2*pin+1:2*pin] -- 00
	GPIO_SWITCH->PUPDR &= ~(BV(SWITCH_PIN*2+1) | BV(SWITCH_PIN*2));
}

int SwitchIsPressed(void) {
	// read switch gpio pin -- IDR[0]
	uint32_t idr = GPIO_SWITCH->IDR;
	// check if pin0 is 0 or 1
	if((idr & BV(SWITCH_PIN)) == 0)
		return 0; // false i.e. swicth not pressed
	return 1; // true i.e. switch is pressed
}











