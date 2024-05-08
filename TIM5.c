#include "TIM5.h"
#define PIN    1



//This function sets up pin PA1 to serve as TIM5_CH2 by providing an alternate function
void configure_PA1(){
  // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   
		
	// 2. Configure GPIO Mode to 'Alternative Function': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(0b11<<(2*PIN));  
	GPIOA->MODER |=   0b10<<(2*PIN);      // AlterFunc(10)

	// 3. Configure GPIO Output Type to 'Push-Pull': Output push-pull (0), Output open drain (1) 
	GPIOA->OTYPER &= ~(1<<PIN);      // Push-pull
	
	// 4. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR  &= ~(0b11<<(2*PIN));  // No pull-up, no pull-down
	
	// 5. Select PA1's alternative function as TIM5_CH2
	GPIOA->AFR[0] &= ~(0xF<<(4*PIN)); //clear bits
	GPIOA->AFR[0] |= 0x2<<4*PIN; //(0010: AF2)
}



//This function sets up the TIM5 timer Channel 2 for Output Compare PWM Mode 1.
void TIM5_CH2_Init(){
	/*Page numbers listed in the comments are from the reference manual*/
		//1. Enable the clock to TIM5 by configuring RCC_APB1ENR1 register
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;

		//2. Configure TIM2 counting mode to upcounting (TIMx_CR1) 
		TIM5->CR1 &= ~(0x07 << 4); //UPCOUNTING mode (CMS+DIR = 000 for edge-aligned): pg 1059
		
		//3. Configure TIM2 Prescalar (TIMx_PSC) and ARR (TIMx_ARR) values
		TIM5->PSC = 3;//399
		TIM5->ARR = 19999; //199
  
		//4. Configure TIMx_CCMR1 CC2S[1:0] bits to output mode for TIM5 Channel 2
		TIM5->CCMR1 &= ~(0x03<<8);//pg981 -> CC1 as output (00)
	
		//5. Configure TIMx_CCMR1 OC2M[3:0] bits to PWM mode 1 for TIM5 Channel 2
		TIM5->CCMR1 &= ~(0xF<<12);//clear bits
		TIM5->CCMR1 |= 6UL<<12;//set to PWM mode 1; Spg 1070
	
    //6. Set TIMx_CCER CC2E bit to enable output signal on Channel 2
		TIM5->CCER |= 1UL<<4; //pg989
    
		//7. Set TIMx_CR1 CEN bit to enable the TIM5 counter
		TIM5->CR1 |= 1UL; //pg1060 -> CEN is bit 0.
		
		//8. Enable the preload register by setting the OCxPE bit in the TIMx_CCMRx register
		TIM5->CCMR1 |= 1UL << 11;//pg 1105
		
		//9. Enable the auto-reload preload register by setting the ARPE bit in the TIMx_CR1 register
		TIM5->CR1 |= 1UL << 7;//pg 1005
}



//Modular function to turn servo +90degrees (ClockWise)
void pos_90degrees(){
	//Pulse Width = 0.5ms 
	TIM5->CCR2 = 1000;

	//Pulse Width = 1ms
	//TIM5->CCR2 = 10;
}



//Modular function to return servo to 0degrees
void _0degrees(){
	//Pulse Width = 1.5ms 
	TIM5->CCR2 = 1500;

	//Pulse Width = 1.5ms
	//TIM5->CCR2 = 15;
}




//Modular function to turn servo -90degrees (Counter Clockwise)
void neg_90degrees(){
	//Pulse Width = 2.5ms 
	TIM5->CCR2 = 2000;

	//Pulse Width = 2ms
	//TIM5->CCR2 = 20;	
}