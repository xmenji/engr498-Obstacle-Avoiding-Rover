#include "TIM4.h"
#include <stdio.h>
//Pins
#define PB6	 6
//Timer ARR & PSC values
//50ms period
#define TIM4_CH1_ARR 499 
#define TIM4_CH1_PSC 399 
#define src_clk 4000000 //4MHZ
//Other const
#define speed_of_sound 0.0343

volatile float echo_pulse_width;			//store the measured input pulse width in microseconds (µs)
volatile unsigned long current_CCR;  //CCR value for falling edge
volatile unsigned long previous_CCR; //CCR value for rising edge
volatile unsigned long N_uev;	//number of overlow events
volatile float fck_cnt = (float)src_clk / (float)(1+TIM4_CH1_PSC); //counter clock period
volatile float distance; //stores the measured detection distance in centimeters (cm).



void configure_PB6(){
  // 1. Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// 2. Configure the GPIO Mode to 'Alternative Function' mode (0b'10)
	//PB6
	GPIOB->MODER &= ~(3UL << 2*PB6);
	GPIOB->MODER |= 2UL << 2*PB6;
	
	// 3. Assign the pin's alternate function to TIM4_CH1; PB6 -> AF2 (b'0010)
	//PB6
	GPIOB->AFR[0] &= ~(0xF << 4*PB6);
	GPIOB->AFR[0] |= 2UL << 4*PB6;
	
}



void configure_TIM4_CH1(){
	// 1. Enable the clock to TIM4 by configuring RCC_APB1ENR1 register
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	// 2. Configure TIM4 counting mode to upcounting (TIMx_CR1)
	TIM4->CR1 &= ~TIM_CR1_DIR;//~(1UL << 4); //set DIR bit=0
	
	// 3. Configure TIM4 Prescalar (TIMx_PSC) and ARR (TIMx_ARR) to generate a desired counter period
	TIM4->PSC = TIM4_CH1_PSC; 
	TIM4->ARR = TIM4_CH1_ARR;
	
	// 4. Configure TIMx_CCMR1 CC1S[1:0] bits for TIM4 Channel 1 to set the Capture/Compare channel
	// direction as input, and input channel (CC1) is mapped on Timer Input 1 (TI1)
	// 01 = input, CC1 is mapped on timer Input 1 (TI1)
	TIM4->CCMR1 &= ~3UL;
	TIM4->CCMR1 |= 1UL;
	
	// 5. Disable digital filtering by configuring TIMx_CCMR1 IC1F[3:0] bits because we want to capture
	// every event
	// 0000: No filter, sampling is done at f DTS
	TIM4->CCMR1 &= ~(0xF<<4);
	
	// 6. Program the input prescaler by configuring TIMx_CCMR1 IC1PSC[1:0] bits. To capture each valid
	// transition, set the input prescaler to zero;
	// 00: no prescaler, capture is done each time an edge is detected on the capture input
	TIM4->CCMR1 &= ~(2UL<<2);
	
	// 7. Select the active edge(s) for trigger of input capture operation by configuring TIMx_CCER CC1NP 
	// and CC1P bits. Consider for this specific system which transition edge(s) need to trigger an input 
	// capture in order to measure the pulse width.
	// CC1NP=1, CC1P=1: both edges
	TIM4->CCER &= ~0xB;//0b'1011
	TIM4->CCER |= 0xA; //0b'1010
	
	// 8. Enable Input Capture mode by setting TIMx_CCER CC1E bit
	//******1: Capture mode enabled / OC1 signal is output on the corresponding output pin???
	TIM4->CCER |= 1UL;
	
	// 9. Enable the necessary interrupts within TIM4 by configuring the TIMx_DIER register. Hint: In this
	// system, you may want to enable the interrupt for the counter overflow event and the input capture on
	// Channel 1.
	// a) Bit 0 UIE: Update interrupt enable; 	1: Update interrupt enabled
	// b) Bit 1 CC1IE: Capture/Compare 1 interrupt enable; 	1: CC1 interrupt enabled.
	// c) Bit 6 TIE: Trigger interrupt enable; 1: Trigger interrupt enabled
	TIM4->DIER |= 0x43;
	
	// 10. Enable TIM4 interrupt source in NVIC. You can use the code below for this step:
	NVIC_EnableIRQ(TIM4_IRQn); // Enable TIM4 interrupt in NVIC
	
	// 11. Set the TIMx_CR1 CEN bit to enable the TIM4 counter. Once enabled, the counter begins counting.
	TIM4->CR1 |= 1UL;

}



void TIM4_IRQHandler(){

	//Input capture event
	if(TIM4->SR & TIM_SR_CC1IF){
		//Get the RISING edge CCR value
		if(GPIOB->IDR & 1UL<<PB6){ 
			previous_CCR = TIM4->CCR1;

		}
		//Get the FALLING edge CCR value
		else if(!(GPIOB->IDR & 1UL<<PB6)){ 
			current_CCR = TIM4->CCR1;
			
			//calculate the distance
			echo_pulse_width = (((float)current_CCR - (float)previous_CCR) + (float)(TIM4_CH1_ARR + 1) * (float)N_uev) * 1000000.0 / fck_cnt;//'1000000' is used to convert s -> us
			distance = (echo_pulse_width * speed_of_sound) / 2;
			
			N_uev = 0; //reset overflow count
			
		}

	}
	//an overflow/underflow event has occurred
	else if (TIM4->SR & TIM_SR_UIF){ 
		N_uev++;//update overflow counter
		
		TIM4->SR &= ~TIM_SR_UIF;//clear interrupt flag
	}

}



//Print the value of 'distance' to USART
void display_distance(){
	char text[] = "\n\rDistance:";
	char distance_c[10];
	int i = 0;

			print(text);
			i=0;
			//cast float value of distance to string
			sprintf(distance_c, "%.2f\n\r", distance);
			//display value of distance
			print(distance_c);
		
}



//returns the current value of distance
float get_distance(){
	return distance;
}