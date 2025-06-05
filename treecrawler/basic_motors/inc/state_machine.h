#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum {
    STATE_IDLE,
    STATE_RUNNING_FORWARD,
    STATE_RUNNING_BACKWARD,
    STATE_RUNNING_TURN_LEFT,
    STATE_RUNNING_TURN_RIGHT,
    STATE_RESETTING,
    STATE_ERROR
} State;

void state_init(void);
State state_handle(void);

#endif