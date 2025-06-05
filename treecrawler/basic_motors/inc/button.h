#ifndef BUTTON_H
#define BUTTON_H


void button_init(void);
void button_handle(void);
EventType button_event(void);

typedef void (*button_callback_t)(ButtonEvent event);
void button_set_callback(button_callback_t cb);

#endif