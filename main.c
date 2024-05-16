
#include <stdio.h>
#include "stm32l476xx.h"
#include "LED.h"
#include "USART2.h"
#include "TIM2.h"
#include "TIM4.h"
#include "push_button.h"
#include "dc_motor.h"

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
	
	char msg[] = "\n\rWelcome! Press the User Button to Start and Stop the System\n\r";;
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
    USART2_Init();
		
		///////////////////////////////////
		//**** MOTOR DRIVER CONFIGS ****///
		///////////////////////////////////

    // Configure motor GPIO pins and initialize PWM timers
    configure_motor_pins();
    setup_timers();

    // Loop to send the welcome message character by character when the system restarts.
    print(msg);
		
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


        distance = get_distance();
        current_distance = distance;
        if(distance <= 50.0){       //Sees obstacle within 50cm
            motor_stop();           // Stop the motors
            delay(1000000);          // Short delay before taking action

            motor_backward();       // Move backward
            delay(500000);          // Move backward for a bit
					
					  motor_stop();           // Stop the motors to take left/right measurements
            delay(1000000);          // Short delay before taking action

            // Take right distance measurement
            pos_90degrees();
            delay(500000);
            display_distance();
            right_distance = get_distance();
            _0degrees();
            delay(500000);

            // Take left distance measurement
            neg_90degrees();
            delay(500000);
            display_distance();
            left_distance = get_distance();
            _0degrees();

            // Decide which way to turn
            if(right_distance >= left_distance){
                motor_turn_right();  // Turn right
                print(turning_right);                
            } else {
                motor_turn_left();   // Turn left
                print(turning_left);
            }

        } else {
            motor_forward();        // Continue moving forward
        }

	}//end while
}//end main

