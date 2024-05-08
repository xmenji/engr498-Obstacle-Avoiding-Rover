#ifndef __STM32L476G_LED_H
#define __STM32L476G_LED_H

#include "stm32l476xx.h"


// Modular function to initialize PA5 as an output pin, interfacing with the LD2 LED.
void configure_LED_pin();

// Modular function to turn on the LD2 LED.
void turn_on_LED();

// Modular function to turn off the LD2 LED.
void turn_off_LED();

// Modular function to toggle the LD2 LED.
void toggle_LED();

#endif /* __STM32L476G_LED_H */