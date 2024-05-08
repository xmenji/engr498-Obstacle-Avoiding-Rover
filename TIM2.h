#ifndef __STM32L476G_TIM2_H
#define __STM32L476G_TIM2_H

#include "stm32l476xx.h"

//This function configures the specified GPIO pin to serve as TIM4_CH1 
//by providing an alternate function.
void configure_PB10();

//configure TIM2 Channel 3 (TIM2_CH3) in Output Compare PWM Mode to
//generate the Trigger signal for the ultrasonic sensor (PB10)
void configure_TIM2_CH3();


#endif