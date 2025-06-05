#include "button.h"
#include <stdio.h>

static EventType event_type;
static bool button_state;
static bool button_state_conf;
static bool last_button_state = 0;
static uint32_t last_toggle;
static uint8_t button_debounce = 10;
static uint8_t GPIOA = 'A';
static uint8_t GPIO_PIN_0 = 0;
static uint32_t press_start_time = 0;

void button_init(void){
    button_state = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
    last_toggle = HAL_GetTick();
}

void button_handle(void){
    button_state = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
    if (button_state != last_button_state){
        last_toggle = HAL_GetTick();
    }
    if (HAL_GetTick() - last_toggle > button_debounce && button_state_conf != button_state){
        button_state_conf = button_state;
        press_start_time = HAL_GetTick() - button_debounce;
    }
    last_button_state = button_state;
}

EventType button_event(void){
    if (button_state_conf && HAL_GetTick() - press_start_time > 1000){
        cb(BUTTON_HOLD);
    }
}