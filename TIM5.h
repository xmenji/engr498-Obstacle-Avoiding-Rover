#ifndef __STM32L476G_TIM5_H
#define __STM32L476G_TIM5_H

#include "stm32l476xx.h"

//This function configures the specified GPIO pin to serve as TIM5_CH2 
//by providing an alternate function.
void configure_PA1();

//This function sets up the TIM5 Module Channel 2 to function in input 
//capture mode to measure the pulse width of the input signal on TIM5_CH2. 
void TIM5_CH2_Init();


//Modular function to turn servo +90degrees (ClockWise)
void pos_90degrees();

//Modular function to return servo to 0degrees
void _0degrees();

//Modular function to turn servo -90degrees (Counter Clockwise)
void neg_90degrees();

#endif