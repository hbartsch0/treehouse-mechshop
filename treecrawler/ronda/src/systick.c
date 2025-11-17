#include "stm32f4xx.h"


volatile uint32_t msTicks = 0; // Global millisecond counter

// This is a standard ARM Cortex-M interrupt handler name.
// The hardware calls this automatically every 1ms if configured.
void SysTick_Handler(void) {
    msTicks++;
}

// Helper function to get the current time
uint32_t GetTickCount(void) {
    return msTicks;
}

// Call this in main() to start the 1ms timer
void SysTick_Init(void) {
    // SystemCoreClock / 1000 gives 1ms interrupts
    SysTick_Config(SystemCoreClock / 1000); 
}

