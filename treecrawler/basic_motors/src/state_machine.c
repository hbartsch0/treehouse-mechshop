/**
 * @file state_machine.c
 * @brief State machine implementation for robot movement control
 *
 * This module implements a finite state machine that controls the robot's
 * movement patterns. It handles transitions between different movement states
 * based on time intervals and (eventually) button inputs.
 */

#include "state_machine.h"
#include <stdio.h>

// Internal state variables
static State state;                        ///< Current state of the robot
static uint32_t last_state_change_time;     ///< Timestamp of last state transition
static uint16_t action_interval = 3000;     ///< Duration for each action/state (ms)

/**
 * @brief Initialize the state machine
 *
 * Sets the initial state to IDLE and initializes the timing variable.
 */
void state_init(void) {
    state = STATE_IDLE;
    last_state_change_time = HAL_GetTick();
    printf("on button press, move to STATE_RUNNING_FORWARD");
}

/**
 * @brief Handle state machine logic and transitions
 *
 * Processes the current state and handles transitions based on time intervals.
 * (Currently, transitions are time-based; should be updated to use button events.)
 *
 * @return State - The current state after processing
 */
State state_handle(void) {
    switch (state) {
        case STATE_IDLE:
            // In IDLE, wait for a button press or timeout to start moving forward
            if (HAL_GetTick() - last_state_change_time > action_interval){ //change to on button press
                printf("moving forward");
                state = STATE_RUNNING_FORWARD;
                last_state_change_time = HAL_GetTick();
            }
            break;

        case STATE_RUNNING_FORWARD:
            // Move forward for 3 seconds, then transition to turning left
            if (HAL_GetTick() - last_state_change_time > action_interval){
                printf("turning left");
                state = STATE_RUNNING_TURN_LEFT;
                last_state_change_time = HAL_GetTick();
            }
            break;

        case STATE_RUNNING_BACKWARD:
            // Move backward for 3 seconds (not implemented)
            break;

        case STATE_RUNNING_TURN_LEFT:
            // Turn left for 3 seconds, then move forward again
            if (HAL_GetTick() - last_state_change_time > action_interval){
                printf("moving forward");
                state = STATE_RUNNING_FORWARD;
                last_state_change_time = HAL_GetTick();
            }
            break;

        case STATE_RUNNING_TURN_RIGHT:
            // Turn right (not implemented)
            break;

        case STATE_STOPPED:
            // System stopped, return to IDLE and stop motors
            break;

        case STATE_RESETTING:
            // Resetting the program, return to IDLE
            state = STATE_IDLE;
            break;

        case STATE_ERROR:
            // System error, return to IDLE and stop motors
            state = STATE_IDLE;
            break;
    }
    return state;
}

/**
 * @brief Button event handler (callback)
 *
 * This function is intended to be registered as the button event callback.
 * It should handle button events and trigger state transitions as needed.
 * (Currently not implemented.)
 */
void state_event_triggered(ButtonEvent event){
    switch (event){
        case HOLD:
            state = STATE_RESETTING;
        case PRESS:
            state = STATE_IDLE;
    }
    event = BUTTON_NONE;
    // TODO: Implement state transitions based on button events
}