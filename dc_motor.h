#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include "stm32l476xx.h"

// Function prototypes
void configure_motor_pins(void);
void motor_stop(void);
void motor_forward(void);
void motor_backward(void);
void motor_turn_left(void);
void motor_turn_right(void);
void control_logic(float left_distance, float right_distance, float current_distance);

#endif // DC_MOTOR_H

