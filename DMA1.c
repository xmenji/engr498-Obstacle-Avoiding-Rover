#include "DMA1.h"
#include <stdlib.h>

uint8_t Buffer_R[6];
uint8_t Buffer_T[6];

//DMA 1, CHANNEL 1
void DMA1_Channel7_Configuration(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	
	//Set the bit size for the transfers
	DMA1_Channel1->CCR &= ~DMA_CCR_PSIZE;
	DMA1_Channel1->CCR &= ~DMA_CCR_MSIZE;
	
	//Increments
	DMA1_Channel1->CCR &= ~DMA_CCR_PINC;
	DMA1_Channel1->CCR |= DMA_CCR_MINC;
	
	//Directions is from peripheral to memory
	DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
	//DMA1_Channel7->CNDTR = 6;
	//Set the address of the peripheral, in this case TIM 4 CH 1
	DMA1_Channel1->CPAR = (uint32_t)&(USART2->TDR);
	//DMA1_Channel7->CMAR = (uint32_t)Buffer_T;
	//DMA1_Channel7->CCR |= DMA_CCR_CIRC;
	
	DMA1_CSELR->CSELR &= ~DMA_CSELR_C1S;
	//Timer 4, Channel 1 : CxS[3:0] = 0110 
	DMA1_CSELR->CSELR |= 0b0110;
	
	//Transfer Complete Interrupt
	DMA1_Channel1->CCR |= DMA_CCR_TCIE;
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	//NVIC_SetPriority(DMA1_Channel7_IRQn, 1);
	//DMA1_Channel7->CCR |= DMA_CCR_EN;
}


void DMA1_Channel1_IRQHandler(void)
{
	if((DMA1->ISR&DMA_ISR_TCIF1)==DMA_ISR_TCIF1)
	{
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
		DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	}
}