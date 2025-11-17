/*
 * systick.h
 *
 * Header file for SysTick timer module.
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f4xx.h"

/* --- Public Function Prototypes --- */

/**
 * @brief Initializes the SysTick timer for 1ms interrupts.
 * Call this in main() to start the 1ms timer.
 * @retval None
 */
void SysTick_Init(void);

/**
 * @brief Gets the current millisecond tick count.
 * @retval Current millisecond counter value
 */
uint32_t GetTickCount(void);

#endif

