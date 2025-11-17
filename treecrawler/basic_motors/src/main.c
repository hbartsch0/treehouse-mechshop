/**
 * @file main.c
 * @brief Main application entry point for the basic motors project
 *
 * This file contains the main function, which initializes the system and registers
 * the button event callback. The main loop should call button_handle and state_handle
 * regularly (not shown in this minimal example).
 */

#include "state_machine.h"
#include "button.h"

int main(void){
    // Register the state machine's event handler as the button callback
    button_set_callback(state_event_triggered);
    // TODO: Add initialization and main loop logic
}