// #include "stm32f4xx.h"
#include "motor_control.h"
#include "uart.h"
#include "systick.h"
#include <stdio.h>
// #include

#define UART_BAUDRATE 9600

// Function declarations
void SystemClock_Config(void);
void GPIO_Config(void);
void Timers_Config(void);

int main(void)
{
    SystemClock_Config();
    uart_init(UART_BAUDRATE);
    uart_write_string("\nInitializing R0nda...");
    // System initialization
    GPIO_Config();
    motor_init();
    SysTick_Init();
    
    
    // Main loop
    uart_write_string("\nHello! I'm r0nda :)");

    static char rx_buffer[32];
    static int rx_index = 0;
    static uint32_t motor_end_time = 0;  // Time when motors should be reset
    static uint8_t motors_active = 0;    // Flag to track if motors are running with a timer
    
    while (1)
    {
        if (!(GPIOC->IDR & (1 << 13))) { // If button is pressed (LOW)
            uart_write_string("\nKILL SWITCH ACTIVATED!");
            motor_set_speed(0, 0, 0);
            motors_active = 0;  // Clear the active flag
            
            // Optional: Wait here until button is released to prevent instant restart
            while (!(GPIOC->IDR & (1 << 13))); 
            continue; // Restart the main loop
        }

        // Check if motor timer has expired
        if (motors_active && (GetTickCount() >= motor_end_time)) {
            motor_set_speed(0, 0, 0);
            motors_active = 0;
            uart_write_string("\nMotor timer expired - motors stopped.");
        }

        if (uart_data_available()) {
            char c = USART2->DR; // Read the character
            uart_write_char(c);       // Echo it back
    
            // If end of line, process the command
            if (c == '\n' || c == '\r') {
                rx_buffer[rx_index] = '\0'; // Terminate string
                uart_write_char('\n');           // New line for user
                
                // --- PARSE COMMAND HERE ---
                int speed = 0, direction = 0, time_ms = 0;
                if (sscanf(rx_buffer, "%d %d %d", &speed, &direction, &time_ms) == 3) {
                    motor_set_speed(speed, direction, time_ms);
                    motor_end_time = GetTickCount() + time_ms;
                    motors_active = 1;
                } else {
                    uart_write_string("Invalid command. Format: speed direction time_ms\n");
                }
                
                rx_index = 0; // Reset buffer for next command
            } 
            // Otherwise, just add to buffer and KEEP LOOPING
            else if (rx_index < sizeof(rx_buffer) - 1) {
                rx_buffer[rx_index++] = c;
            }
        }


    }
    
    return 0;
}

/**
 * @brief  System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    // --- 1. Set Flash Latency ---
    // We're setting the LATENCY bits in the Flash Access Control Register
    // to 2 (binary 0b010). The header file gives us a name for this:
    // FLASH_ACR_LATENCY_2WS
    FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

    // --- 2. Configure PLL ---
    // We set the source, M, N, and P values all in one go.
    // We use bitwise OR (|) to combine all the settings.
    RCC->PLLCFGR = (RCC_PLLCFGR_PLLSRC_HSI | 
                    (16 << RCC_PLLCFGR_PLLM_Pos) |
                    (336 << RCC_PLLCFGR_PLLN_Pos) |
                    (1 << RCC_PLLCFGR_PLLP_Pos));

    // --- 3. Enable the PLL ---
    // Set the PLLON (PLL On) bit in the Control Register
    RCC->CR |= RCC_CR_PLLON;
    // Wait until the PLLRDY (PLL Ready) bit is set
    // The '!' inverts the logic, so we "wait while not ready
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // --- 4. Configure AHB/APB Prescalers ---
    // Set HPRE (AHB Prescaler) to /1 (84MHz / 1 = 84MHz)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    // Set PPRE1 (APB1 Prescaler) to /2 (84MHz / 2 = 42MHz)
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    // Set PPRE2 (APB2 Prescaler) to /1 (84MHz / 1 = 84MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    // --- 5. Switch System Clock to PLL ---
    // Set the SW (Switch) bits to use the PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    // Wait until the SWS (Switch Status) bits show PLL is in use
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);
}

/**
 * @brief  GPIO Configuration
 * @retval None
 */
void GPIO_Config(void)
{
    // --- 1. Enable GPIO Clocks ---
    // We need to power on the GPIO "ports" we want to use.
    // Both are on the AHB1 bus.
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // --- 2. Configure Button Pins (PC10, PC13) ---
    
    // PC13 (On-board User Button)
    // We set its mode to Input (00). We do this by "clearing" its
    // two bits in the MODER register.
    // 'GPIO_MODER_MODER13' is a mask (0b11) at PC13's position.
    // '~' inverts it to (0b...1100...11), so we 'AND' to clear.
    GPIOC->MODER &= ~(GPIO_MODER_MODER13);
    // PC10 (External Button)
    // Same as above, set it to Input (00)
    GPIOC->MODER &= ~(GPIO_MODER_MODER10);

    // Configure PA5 as Output (01)
    GPIOA->MODER |= GPIO_MODER_MODER5_0;  // Set bit 0
    GPIOA->MODER &= ~GPIO_MODER_MODER5_1; // Clear bit 1

    // Set PC10 to Pull-up (01)
    // We use |= to set the first bit (PUPD10_0)
    GPIOC->PUPDR |= (GPIO_PUPDR_PUPD10_0);

    // --- 3. Configure PWM Pins (PA0, PA1, PA6, PA7) ---
    
    // Set Mode to Alternate Function (10) for all 4 pins
    // We 'OR' all the settings together in one command.
    GPIOA->MODER |= (GPIO_MODER_MODER0_1 | //PA0
                    GPIO_MODER_MODER1_1 | //PA1
                    GPIO_MODER_MODER6_1 | //PA6
                    GPIO_MODER_MODER7_1); //PA7

    // Set the specific Alternate Function (AF)
    // We use AFR[0] because all our pins are 0-7
    
    // Set PA0 to AF1 (TIM2_CH1)
    GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos);

    // Set PA1 to AF1 (TIM2_CH2)
    GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL1_Pos); 
    
    // Set PA6 to AF2 (TIM3_CH1)
    GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos); 
    
    // Set PA7 to AF2 (TIM3_CH2)
    GPIOA->AFR[0] |= (2 << GPIO_AFRL_AFSEL7_Pos);

}

