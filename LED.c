#include "LED.h"

// PA5  <--> Green LED
#define LED_PIN    5


void configure_LED_pin(){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// 2. Configure GPIO Mode to 'Output': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(3UL<<(2*LED_PIN));  
	GPIOA->MODER |=   1UL<<(2*LED_PIN);      // Output(01)

	// 3. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<LED_PIN);      // Push-pull
	
	// 4. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(0b11<<(2*LED_PIN));  // No pull-up, no pull-down
}

// Modular function to turn on the LD2 LED.
void turn_on_LED(){
	GPIOA->ODR |= 1 << LED_PIN;
}

// Modular function to turn off the LD2 LED.
void turn_off_LED(){
	GPIOA->ODR &= ~(1 << LED_PIN);
}

// Modular function to toggle the LD2 LED.
void toggle_LED(){
	GPIOA->ODR ^= (1 << LED_PIN);
}
