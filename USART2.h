#ifndef __STM32L476G_USART2_H
#define __STM32L476G_USART2_H

#include "stm32l476xx.h"


// This function initializes the USART2 module
void USART2_Init(void);

// This function initializes the GPIO pins used for USART2 communication.
void USART2_Pin_Init(void);

// This function initializes USART module with specified settings for communication.
// This function is modular and can be utilized with any USART module passed as an argument.
void USART_Init(USART_TypeDef * USARTx);

#endif /* __STM32L476G_USART2_H */