#include "motor_control.h"
#include "systick.h"
// #include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>

#define MOTOR_L 1
#define MOTOR_R 2

#define MAX_SPEED 1000
#define MIN_SPEED -1000

/**
 * @brief  Timers Configuration
 * @retval None
 */
 void motor_set_speed(int speed, int turn, int time_ms){
    /*TODO
    - scale speed & turn speed properly
    - 
    */

    /*NOTES
        - speed is +/-
    */

    char buffer[32]; // Make sure this is big enough!

    snprintf(buffer, sizeof(buffer), "\nS: %d%%; D: %d%% for %d ms", speed, turn, time_ms);
    uart_write_string(buffer);

    speed = speed*10;
    turn = turn*10;

    int motor_l_speed = speed + turn;
    int motor_r_speed = speed - turn;

    if (motor_l_speed > MAX_SPEED){
        motor_l_speed = MAX_SPEED;
    } else if (motor_l_speed < MIN_SPEED) {
        motor_l_speed = MIN_SPEED;
    }

    if (motor_r_speed > MAX_SPEED){
        motor_r_speed = MAX_SPEED;
    } else if (motor_r_speed < MIN_SPEED) {
        motor_r_speed = MIN_SPEED;
    }

    TIM2->CCR1 = motor_l_speed;
    TIM3->CCR1 = motor_r_speed;
    uart_write_string("\nMotor speeds set!");

    
    // int avg_speed = (motor_l_speed + motor_r_speed)/2;
 }

uint32_t last_blink_time = 0;
int led_state = 0;
void motor_led(int led_speed){
    uint32_t blink_delay = 1000 - (led_speed * 9); // Range: ~100ms to 1000ms
    if (led_speed == 0) blink_delay = 2000; // Very slow blink when stopped

    // Non-blocking blink check (using a simple tick counter)
    // You'll need a generic 'millis()' function for this to work perfectly bare-metal.
    // For now, we can use a simple loop counter as a crude approximation
    // if you don't have SysTick set up yet.
    if ((GetTickCount() - last_blink_time) > blink_delay) {
        last_blink_time = GetTickCount();
        led_state = !led_state;
        if (led_state) {
            GPIOA->ODR |= (1 << 5); // Turn LED ON (PA5 HIGH)
        } else {
            GPIOA->ODR &= ~(1 << 5); // Turn LED OFF (PA5 LOW)
        }
    }
}

/**
 * @brief  Timers Configuration
 * @retval None
 */
 void motor_init(void)
 {
     // --- 1. Enable Timer Clocks ---
     // Timers 2 and 3 are on the APB1 bus.
     RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
     RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
 
     // --- 2. Configure TIM2 (for Motor 1) ---
     // Set Prescaler
     TIM2->PSC = 83;
     // Set Auto-Reload Register (the period)
     TIM2->ARR = 999;
     // Set Channel 1 and 2 to "PWM Mode 1"
     TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
     TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1);
     // Enable Preload for all channels (good practice, means
     // duty cycle changes only take effect at the start of a new period)
     TIM2->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
     // Set initial Duty Cycle (0%)
     TIM2->CCR1 = 0;
     TIM2->CCR2 = 0;
     // Enable the outputs for Channel 1 and 2
     TIM2->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
     // Enable the timer counter!
     TIM2->CR1 |= TIM_CR1_CEN;
 
     TIM3->PSC = 83;
     TIM3->ARR = 999;
     TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
     TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1);
     TIM3->CCMR1 |= (TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
     TIM3->CCR1 = 0;
     TIM3->CCR2 = 0;
     TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
     TIM3->CR1 |= TIM_CR1_CEN;
 
 }