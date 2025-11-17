/*
 * motor_control.h
 *
 * Header file for motor control module.
 */

// These are "header guards." They prevent this file
// from being included more than once, which can
// cause compilation errors.

#ifndef MOTOR_CONTROL_H_ //If not defined
#define MOTOR_CONTROL_H_

#include "stm32f4xx.h"

/* --- Public Function Prototypes --- */

/**
 * @brief Initializes the timers (TIM2, TIM3) for PWM.
 * This replaces the old Timers_Config() function.
 */
void motor_init(void);

/**
 * @brief Sets the speed and turn rate for both motors.
 * @param speed: Forward/backward speed (-100 to 100)
 * @param turn: Turn rate (-100 to 100), positive turns right
 * @param time_ms: Duration in milliseconds (currently unused in function, used by caller for timing)
 */
 void motor_set_speed(int speed, int turn, int time_ms);

/**
 * @brief Flashes the onboard led proportional to the speed of the motors
 * @param led_speed: blinking speed of led
 */
 void motor_led(int led_speed);


 #endif