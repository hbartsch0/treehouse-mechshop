#include "uart.h"
#include "stm32f4xx.h"

/**
 * @brief  UART2 Initialization
 * @param  baudrate: Desired baud rate
 * @retval None
 */
void uart_init(uint32_t baudrate)
{
    // --- 1. Enable Clocks ---
    // Enable GPIOA clock (UART2 uses PA2/PA3)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // Enable USART2 clock (on APB1 bus)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // --- 2. Configure GPIO Pins (PA2 = TX, PA3 = RX) ---
    // Set PA2 and PA3 to Alternate Function mode (10)
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
    // Clear the lower bit to ensure alternate function mode
    GPIOA->MODER &= ~(GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0);

    // Set PA2 and PA3 to AF7 (USART2)
    // PA2 is in AFR[0], bits 8-11
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos);
    // PA3 is in AFR[0], bits 12-15
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL3_Pos);

    // --- 3. Configure UART ---
    // Disable UART before configuration
    USART2->CR1 &= ~USART_CR1_UE;

    // Calculate baud rate divisor
    // For APB1 at 42MHz: USARTDIV = 42000000 / (16 * baudrate)
    // BRR = USARTDIV * 16 (in fixed point, with 4 fractional bits)
    uint32_t usartdiv = (42000000 / (16*baudrate))*16;
    // uint32_t usartdiv = (42000000 + (baudrate / 2)) / baudrate;
    USART2->BRR = usartdiv;

    // Configure: 8 data bits, 1 stop bit, no parity (default)
    // Enable Transmitter and Receiver
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    // Enable UART
    USART2->CR1 |= USART_CR1_UE;
}

/**
 * @brief  Send a single character via UART2
 * @param  data: Character to send
 * @retval None
 */
void uart_write_char(uint8_t data)
{
    // Wait until Transmit Data Register is Empty
    while (!(USART2->SR & USART_SR_TXE));
    // Write data to Data Register
    USART2->DR = data;
    // Wait until transmission is complete
    while (!(USART2->SR & USART_SR_TC));
}

/**
 * @brief  Send a string via UART2
 * @param  str: Null-terminated string to send
 * @retval None
 */
void uart_write_string(const char *str)
{
    while (*str)
    {
        uart_write_char(*str);
        str++;
    }
}

/**
 * @brief  Receive a single character via UART2
 * @retval Received character, or 0 if no data available
 */
 int uart_read_line(char* buffer, int max_len) {
    int count = 0;
    while (count < max_len - 1) {
        // Wait for a character to be received (RXNE bit)
        while (!(USART2->SR & USART_SR_RXNE)) {}
        
        char c = USART2->DR;
        
        // Echo the character back to the user (optional, but helpful)
        uart_write_char(c);

        // Check for end of line (Enter key)
        if (c == '\n' || c == '\r') {
            buffer[count] = '\0'; // Null-terminate the string
            uart_write_char('\n');     // Send a clean new line
            return count;
        }
        
        buffer[count++] = c;
    }
    buffer[max_len - 1] = '\0'; // Ensure null-termination if buffer is full
    return count;
}

/**
 * @brief  Check if data is available to receive
 * @retval 1 if data is available, 0 otherwise
 */
uint8_t uart_data_available(void)
{
    return (USART2->SR & USART_SR_RXNE) ? 1 : 0;
}
