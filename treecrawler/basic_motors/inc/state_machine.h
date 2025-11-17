/**
 * @file state_machine.h
 * @brief State machine interface for robot movement control
 * 
 * This module implements a finite state machine that controls the robot's
 * movement patterns. It handles transitions between different movement states
 * based on time intervals and button inputs.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
 * @brief Robot movement states
 * 
 * Defines all possible states for the robot's movement control system.
 * Each state represents a different movement pattern or system condition.
 */
typedef enum {
    STATE_IDLE,              ///< Robot is stopped and waiting for input
    STATE_RUNNING_FORWARD,   ///< Robot is moving forward
    STATE_RUNNING_BACKWARD,  ///< Robot is moving backward
    STATE_RUNNING_TURN_LEFT, ///< Robot is turning left
    STATE_RUNNING_TURN_RIGHT,///< Robot is turning right
    STATE_RESETTING,         ///< Robot is resetting its position/state
    STATE_ERROR              ///< Robot is in error state (should stop motors)
} State;

/**
 * @brief Initialize the state machine
 * 
 * Sets up the initial state (IDLE) and initializes timing variables.
 * Must be called before using any other state machine functions.
 */
void state_init(void);

/**
 * @brief Handle state machine logic and transitions
 * 
 * Processes the current state and handles transitions based on:
 * - Time intervals (automatic state changes)
 * - Button events (manual state changes)
 * - Error conditions
 * 
 * This function should be called regularly in the main loop.
 * 
 * @return State - The current state after processing
 */
State state_handle(void);

void state_event_triggered(ButtonEvent event);

#endif