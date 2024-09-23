/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 19, 2024
*/

#ifndef SWITCH_H_
#define SWITCH_H_

#include "stm32f4xx.h"

#define GPIO_SWITCH		GPIOA
#define SWITCH_PIN		0

#define GPIO_SWITCH_CLKEN	0

void SwitchInit(void);
int SwitchIsPressed(void);

#endif /* SWITCH_H_ */



