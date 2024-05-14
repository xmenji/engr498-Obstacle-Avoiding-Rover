//Lab 7 Code
#include <stdio.h>
#include "stm32l476xx.h"
#include "LED.h"
#include "USART2.h"
#include "TIM2.h"
#include "TIM4.h"
#include "push_button.h"

volatile float right_distance;
volatile float left_distance;
volatile float current_distance;

//modular function to add delays within the program
void delay(int n){
	int i;
	for(i = 0; i <= n; i++);
}



//modular function to print messages to terminal via USART
void print(char* msg){
	int i = 0;
	// Loop to send the welcome message character by character when the system restarts.
	while(msg[i]!='\0'){
		
		// Wait until the TXE (Transmit Data Register Empty) flag is set, indicating readiness to send the next character.
		while (!(USART2->ISR & USART_ISR_TXE));   	
		
		// Send the current character and automatically clear the TXE flag by writing to USART_TDR.
		// increment i after sending the current character
		USART2->TDR = msg[i++];
	}
}
	


/////////////////////////////
// *****MAIN FUNCTION******//
/////////////////////////////
int main(void){
	
	char msg[] = "\n\rWelcome! Press '1' to start the distance detection and measurement; press '2' to stop the system.\n\r";
	char turning_right[] = "\n\rTurning right...\n\r";
	char turning_left[] = "\n\rTurning left...\n\r";
	char moving_forward[] = "\n\rMoving forward...\n\r";
	int i=0;
  //////////////////////////////////////////////
	/**** ULTRA SONIC SENSOR MODULE CONFIGS ****/
	/////////////////////////////////////////////
	
	// Configure PA5 to serve as an output pin, used for controlling the on-board LED (LD2).
	configure_LED_pin();
	
	//Configure PB10 as TIM2 Channel 3 (Output Trigger)
	configure_PB10();
	
	//Configure PB6 as TIM4 Channel 1 (Input Echo)
	configure_PB6();
	
	//Configure TIM4 CH1 as input capture mode
	configure_TIM4_CH1();
	
	//Configure TIM2 CH3 as PWM output mode 1
	configure_TIM2_CH3();
	
	// Initialize USART2 for communication with a PC via serial terminal.
	// Configuration details:
	// - Data format: 8 data bits, no parity, 1 start bit, and 1 stop bit
	// - Baud rate: 9600
	// - Enable RXNE interrupt to trigger when a new character is received, allowing for responsive character echo.
	USART2_Init();
	
	// Loop to send the welcome message character by character when the system restarts.
  print(msg);
	
	 ///////////////////////////////////
	//**** SERVO MODULE CONFIGS ****///
	///////////////////////////////////
	//Configure pin PA1
	configure_PA1();
	
	//Initialize Timer 5, Channel 2
	TIM5_CH2_Init();
	
	///////////////////////////////////
	//**** PUSH BUTTON CONFIGS ****///
	///////////////////////////////////
  //Invoke configure_Push_Button_pin() to initialize PC13 as an input pin, interfacing with the USER push button.
	configure_Push_Button_pin();
	//Invoke configure_EXTI() to set up edge-triggered interrput on PC13
	configure_EXTI();
	
	
	float distance;

	// Enter an infinite loop to maintain the program's operation.
	while (1){
		//User presses '1' on the keyboard
		if(USART2->RDR == '1'){
			//turn_on_LED();			//turn on the on-board LED
			//display_distance(); //display distance measurement to terminal
			delay(100000);						//add a delay between each print
		}
		
		distance = get_distance();
		if(distance <= 9){
			//rover stops moving
			current_distance = distance;
			//rover moves backwards a few cm
			
			//rover stops moving
			
			//rover looks right; takes measurement
			pos_90degrees();
			delay(500000);
			display_distance();
			right_distance = get_distance();
			_0degrees();
			delay(500000);
			
			//rover looks left; takes measurement
			neg_90degrees();
			delay(500000);
			display_distance();
			left_distance = get_distance();
			_0degrees();
			
			
			//rover decides which way to turn
			if(right_distance > left_distance){
				//rover turns right
				print(turning_right);				
			}
			else{
				//rover turns left
				print(turning_left);
			}

		}
		else{
			//keep moving forward
			//print(moving_forward);
			//display_distance();
		}

	}//end while
}//end main

