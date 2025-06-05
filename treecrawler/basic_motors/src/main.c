#include "state_machine.h"
#include "button.h"

void main(void){
    button_set_callback(state_event_triggered);
}