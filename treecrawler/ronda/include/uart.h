/*
 * uart.h
 *
 * Header file for UART communication module.
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"

/* --- Public Function Prototypes --- */

/**
 * @brief Initializes UART2 for communication.
 * @param baudrate: Desired baud rate (e.g., 9600, 115200)
 * @retval None
 */
void uart_init(uint32_t baudrate);

/**
 * @brief Sends a single character via UART2.
 * @param data: Character to send
 * @retval None
 */
void uart_write_char(uint8_t data);

/**
 * @brief Sends a string via UART2.
 * @param str: Null-terminated string to send
 * @retval None
 */
void uart_write_string(const char *str);

/**
 * @brief Receives a single character via UART2.
 * @retval Received character, or 0 if no data available
 */
int uart_read_line(char* buffer, int max_len);

/**
 * @brief Checks if data is available to receive.
 * @retval 1 if data is available, 0 otherwise
 */
uint8_t uart_data_available(void);

#endif

