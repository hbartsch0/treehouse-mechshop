#include "state_machine.h"
#include <stdio.h>

static State state;
static uint32_t last_state_change_time;
static uint16_t action_interval = 3000;

void state_init(void) {
    state = STATE_IDLE;
    last_state_change_time = HAL_GetTick();
    printf("on button press, move to STATE_RUNNING_FORWARD");
}

State state_handle(void) {
    switch (state) {
        case STATE_IDLE:
            
            if (HAL_GetTick() - last_state_change_time > action_interval){ //change to on button press
                printf("moving forward");
                state = STATE_RUNNING_FORWARD;
                last_state_change_time = HAL_GetTick();
            }
            
            break;

        case STATE_RUNNING_FORWARD:
            // move forward for 3 seconds then STATE_RUNNING_TURN_LEFT
            
            if (HAL_GetTick() - last_state_change_time > action_interval){
                printf("turning left");
                state = STATE_RUNNING_TURN_LEFT;
                last_state_change_time = HAL_GetTick();
            }
            break;

        case STATE_RUNNING_BACKWARD:
            // move backward for 3 seconds
            break;

        case STATE_RUNNING_TURN_LEFT:
        // turn left
            
            if (HAL_GetTick() - last_state_change_time > action_interval){
                printf("moving forward");
                state = STATE_RUNNING_FORWARD;
                last_state_change_time = HAL_GetTick();
            }
            
            break;

        case STATE_RUNNING_TURN_RIGHT:
            // turn right
            break;

        case STATE_STOPPED:
            // printf("system stopped.")
            state = STATE_IDLE;
            // stop the motors
            break;

        case STATE_RESETTING:
            // printf("resetting the program.");
            state = STATE_IDLE;
            // reset the program
            break;

        case STATE_ERROR:
            // printf("system error :(");
            state = STATE_IDLE;
            // stop the motors
            break;
    }
    return state;
}

void button_event_triggered(void){
    
}