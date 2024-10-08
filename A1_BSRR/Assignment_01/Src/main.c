/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	SystemInit();

	// Enable GPIOD clock -- RCC AHB1ENR[3] = 1
	RCC -> AHB1ENR |= BV(3);

	// Configure GPIOD.13 as output -- MODER[27:26] = 01
	GPIOD -> MODER &= ~BV(27);
	GPIOD -> MODER |= BV(26);

	// Configure GPIOD.13 as push-pull -- OTYPER[13] = 0
	GPIOD -> OTYPER &= ~BV(13);

	// Configure GPIOD.13 as low-speed -- OSPEEDR[27:26] = 00
	GPIOD -> OSPEEDR &= ~(BV(27));
	GPIOD -> OSPEEDR |= BV(26);

	// Configure GPIOD.13 with no pull-up/pull-down resistors -- PUPDR[27:26] = 00
	GPIOD -> PUPDR &= ~(BV(27));
	GPIOD -> PUPDR |= BV(26);

	while(1)
	{
		// Turn LED ON by setting GPIOD.13 -- BSRR[13] = 1
		GPIOD -> BSRR |= BV(13);
		DelayMs(500);  // Delay for 500ms

		// Turn LED OFF by resetting GPIOD.13 -- BSRR[29] = 1
		GPIOD -> BSRR |= BV(29);
		DelayMs(500);  // Delay for 500ms
	}

	return 0;
}
