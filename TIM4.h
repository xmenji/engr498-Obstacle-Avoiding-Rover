#ifndef __STM32L476G_TIM4_H
#define __STM32L476G_TIM4_H

#include "stm32l476xx.h"

//This function configures the specified GPIO pin to serve as TIM4_CH1 
//by providing an alternate function.
void configure_PB6();

//This function sets up the TIM4 Module Channel 1 to function in input 
//capture mode to measure the pulse width of the input signal on TIM4_CH1. (PB6)
void configure_TIM4_CH1();

//Timer 4 Interrupt handler monitors pin (PB6) for echo pulse signals
//and calculates the distance based on the width of those signals
void TIM4_IRQHandler();


void display_distance();

float get_distance();

#endif