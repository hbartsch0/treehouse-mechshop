/**
 * @file button.h
 * @brief Button input handling interface for the basic motors project
 * 
 * This module provides button input handling with debouncing and event detection.
 * It supports both press/release events and hold detection for long button presses.
 */

#ifndef BUTTON_H
#define BUTTON_H

/**
 * @brief Initialize the button module
 * 
 * Sets up the button GPIO pin and initializes internal state variables.
 * Must be called before using any other button functions.
 */
void button_init(void);

/**
 * @brief Handle button state changes and debouncing
 * 
 * This function should be called regularly (e.g., in the main loop) to:
 * - Read the current button state
 * - Apply debouncing to prevent false triggers
 * - Update internal state tracking
 */
void button_handle(void);

/**
 * @brief Check for button events and trigger callbacks
 * 
 * Processes button events and calls the registered callback function
 * when events are detected (press, release, hold).
 * 
 * @return EventType - The type of event that occurred (if any)
 */
EventType button_event(void);

/**
 * @brief Callback function type for button events
 * 
 * @param event The button event that occurred
 */
typedef void (*button_callback_t)(ButtonEvent event);

typedef enum {
    BUTTON_NONE,
    BUTTON_PRESS,
    BUTTON_HOLD
} ButtonEvent;

/**
 * @brief Register a callback function for button events
 * 
 * Sets the function that will be called when button events are detected.
 * The callback will be called with the appropriate event type.
 * 
 * @param cb Pointer to the callback function
 */
void button_set_callback(button_callback_t cb);

#endif