#include "TIM2.h"

#define PB10 10

//100ms period
#define TIM2_CH3_ARR 39 
#define TIM2_CH3_PSC 9 



void configure_PB10(){
  // 1. Enable the clock to GPIO Port B	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	// 2. Configure the GPIO Mode to 'Alternative Function' mode (0b'10)

	//PB10
	GPIOB->MODER &= ~(3UL << 2*PB10);
	GPIOB->MODER |= 2UL << 2*PB10; //0b'10
	
	// 3. Assign the pin's alternate function to TIM4_CH1; PB6 -> AF2 (b'0010)

	//PB10 (AFRH[2]: AF1 = 0001
	GPIOB->AFR[1] &= ~(0xF << 4*2);
	GPIOB->AFR[1] |= 1UL << 4*2;	
	
	//Steps4&5 are PB10-specific since it is in output mode
	// 4. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOB->OTYPER &= ~(1<<PB10);      // Push-pull
	
	// 5. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR  &= ~(0b11<<(2*PB10));  // No pull-up, no pull-down
}




void configure_TIM2_CH3(){
	
		//1. Enable the clock to TIM5 by configuring RCC_APB1ENR1 register
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

		//2. Configure TIM2 counting mode to upcounting (TIMx_CR1) 
		TIM2->CR1 &= ~(0x07 << 4); //UPCOUNTING mode (CMS+DIR = 000 for edge-aligned): pg 1059
		
		//3. Configure TIM2 Prescalar (TIMx_PSC) and ARR (TIMx_ARR) values
		TIM2->PSC = TIM2_CH3_PSC; 
		TIM2->ARR = TIM2_CH3_ARR; 
	  TIM2->CCR3 = 4; //10us pulse width
	
		//4. Configure TIMx_CCMR2 CC3S[1:0] bits to output mode for TIM2 Channel 3
		TIM2->CCMR2 &= ~0x03;//output (00)
	
		//5. Configure TIMx_CCMR2 OC3M[3:0] bits to PWM mode 1 for TIM2 Channel 3
		TIM2->CCMR2 &= ~(0x7<<4);
		TIM2->CCMR2 |= 0x6<<4;
	
    //6. Set TIMx_CCER CC2E bit to enable output signal on Channel 3
		TIM2->CCER |= 1UL<<8;
    
		//7. Set TIMx_CR1 CEN bit to enable the TIM2 counter
		TIM2->CR1 |= 1UL; 
		
		//8. Enable the preload register by setting the OCxPE bit in the TIMx_CCMRx register
		TIM2->CCMR2 |= 1UL << 3;
		
		//9. Enable the auto-reload preload register by setting the ARPE bit in the TIMx_CR1 register
		TIM2->CR1 |= 1UL << 7;

}