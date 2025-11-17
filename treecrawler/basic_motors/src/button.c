/**
 * @file button.c
 * @brief Button input handling implementation for basic motors project
 *
 * This module implements button input handling with debouncing and event detection.
 * It reads from GPIO pin A0 and provides debounced button state with event callbacks.
 */

#include "button.h"
#include <stdio.h>

#define BUTTON_PORT GPIOA //GPIO_TypeDef *
#define BUTTON_PIN GPIO_PIN_0

// Internal state variables for button handling
static EventType event_type;           ///< Current event type being processed
static bool button_state;              ///< Current raw button state from GPIO
static bool button_state_conf;         ///< Debounced/confirmed button state
static bool last_button_state = 0;     ///< Previous button state for edge detection
static uint32_t last_toggle;           ///< Timestamp of last button state change
static uint8_t button_debounce = 10;   ///< Debounce time in milliseconds
static uint32_t press_start_time = 0;  ///< Timestamp when button press started
static button_callback_t callback = NULL; ///< Callback function pointer (set by button_set_callback)



/**
 * @brief Initialize the button module
 *
 * Sets up initial button state and timing variables.
 * Reads the current button state to establish baseline.
 */
void button_init(void) {
    button_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN); // Read initial button state
    last_toggle = HAL_GetTick();                        // Initialize timing variable
}

/**
 * @brief Handle button state changes and debouncing
 *
 * This function implements the debouncing algorithm:
 * 1. Read current button state
 * 2. Detect state changes and update timing
 * 3. Apply debounce delay before confirming state changes
 * 4. Track press duration for hold detection
 */
void button_handle(void) {
    button_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN); // Read current button state
    if (button_state != last_button_state) {            // Detect state change
        last_toggle = HAL_GetTick();                    // Update timestamp
    }
    if (HAL_GetTick() - last_toggle > button_debounce && button_state_conf != button_state) {
        button_state_conf = button_state;               // Confirm state after debounce
        press_start_time = HAL_GetTick() - button_debounce; // Record press start time
    }
    last_button_state = button_state;                   // Update previous state
}

/**
 * @brief Check for button events and trigger callbacks
 *
 * Processes button events and calls the registered callback function.
 * Currently only handles hold events (button pressed for >1s).
 *
 * @return EventType - The type of event that occurred (if any)
 */
EventType button_event(void) {
    if (button_state_conf && HAL_GetTick() - press_start_time > 1000) {
        if (callback != NULL) {
            callback(BUTTON_HOLD); // Call the registered callback for hold event
        }
    } else if (press_start_time - last_toggle > button_debounce && press_start_time - last_toggle < 1000) {
        if (callback != NULL) {
            callback(BUTTON_PRESS); // Call the registered callback for press and release event
        }
    }
    // TODO: Add press and release event detection and proper event return mechanism
    return 0; // Placeholder return
}

/**
 * @brief Register a callback function for button events
 *
 * Sets the function that will be called when button events are detected.
 * The callback will be called with the appropriate event type.
 *
 * @param cb Pointer to the callback function
 */
void button_set_callback(button_callback_t cb) {
    callback = cb;
}