#include "stm32l476xx.h"
#include "dc_motor.h"

// GPIO Pin Definitions for Motor Control
#define IN1_PIN 1<<4  // PB4 (D3)
#define IN2_PIN 1<<5  // PB5 (D5)
#define IN3_PIN 1<<10 // PB10 (D6)
#define IN4_PIN 1<<0  // PA0 (D9, TIM2_CH1 for PWM)

// Function to configure GPIO pins for the motor driver
void configure_motor_pins() {
    // Enable Clock for GPIOA and GPIOB
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // Set PB4, PB5, and PB10 as output
    GPIOB->MODER &= ~((0x3 << (4 * 2)) | (0x3 << (5 * 2)) | (0x3 << (10 * 2)));  // Clear mode bits
    GPIOB->MODER |= (0x1 << (4 * 2)) | (0x1 << (5 * 2)) | (0x1 << (10 * 2));  // Set mode to output

    // Set PA0 as alternate function (TIM2_CH1)
    GPIOA->MODER &= ~(0x3 << (0 * 2));  // Clear mode bits
    GPIOA->MODER |= (0x2 << (0 * 2));  // Set mode to AF
    GPIOA->AFR[0] |= (1 << (0 * 4));  // Set AF1 for TIM2_CH1
}

// Timer setup for PWM control
void setup_timers() {
    // Enable clock for TIM2 and TIM3
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;

    // Configure TIM3 for PWM mode
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;  // PWM mode 1 on Channel 1
    TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;  // PWM mode 1 on Channel 2
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;  // Enable output on Channel 1 and 2
    TIM3->PSC = 79;  // Prescaler to 1 MHz
    TIM3->ARR = 999;  // Auto-reload at 1 kHz
    TIM3->CR1 |= TIM_CR1_CEN;  // Enable TIM3

    // Configure TIM2 for PWM mode
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;  // PWM mode 1 on Channel 1
    TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;  // PWM mode 1 on Channel 3
    TIM2->CCER |= TIM_CCER_CC1E | TIM_CCER_CC3E;  // Enable output on Channel 1 and 3
    TIM2->PSC = 79;  // Prescaler to 1 MHz
    TIM2->ARR = 999;  // Auto-reload at 1 kHz
    TIM2->CR1 |= TIM_CR1_CEN;  // Enable TIM2
}

// Motor control functions using GPIO adjustments and PWM
void motor_stop() {
    TIM3->CCR1 = 0;  // Set PWM duty cycle to 0% (no power)
    TIM3->CCR2 = 0;
    TIM2->CCR1 = 0;
    TIM2->CCR3 = 0;
}

void motor_forward() {
    GPIOB->ODR |= IN1_PIN;  // Set IN1 to HIGH
    GPIOB->ODR &= ~IN2_PIN; // Set IN2 to LOW
    GPIOB->ODR |= IN3_PIN;  // Set IN3 to HIGH
    GPIOA->ODR &= ~IN4_PIN; // Set IN4 to LOW
    TIM3->CCR1 = 500;  // Set PWM duty cycle to 50%
    TIM3->CCR2 = 0;
    TIM2->CCR1 = 500;
    TIM2->CCR3 = 0;
}

void motor_backward() {
    GPIOB->ODR &= ~IN1_PIN; // Set IN1 to LOW
    GPIOB->ODR |= IN2_PIN;  // Set IN2 to HIGH
    GPIOB->ODR &= ~IN3_PIN; // Set IN3 to LOW
    GPIOA->ODR |= IN4_PIN;  // Set IN4 to HIGH
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 500;  // Set PWM duty cycle to 50%
    TIM2->CCR1 = 0;
    TIM2->CCR3 = 500;
}

void motor_turn_left() {
    GPIOB->ODR |= IN1_PIN;  // Set IN1 to HIGH
    GPIOB->ODR &= ~IN2_PIN; // Set IN2 to LOW
    GPIOB->ODR &= ~IN3_PIN; // Set IN3 to LOW
    GPIOA->ODR |= IN4_PIN;  // Set IN4 to HIGH
    TIM3->CCR1 = 250;  // Set PWM duty cycle to 25%
    TIM3->CCR2 = 500;
    TIM2->CCR1 = 0;
    TIM2->CCR3 = 500;
}

void motor_turn_right() {
    GPIOB->ODR &= ~IN1_PIN; // Set IN1 to LOW
    GPIOB->ODR |= IN2_PIN;  // Set IN2 to HIGH
    GPIOB->ODR |= IN3_PIN;  // Set IN3 to HIGH
    GPIOA->ODR &= ~IN4_PIN; // Set IN4 to LOW
    TIM3->CCR1 = 500;  // Set PWM duty cycle to 50%
    TIM3->CCR2 = 250;
    TIM2->CCR1 = 500;
    TIM2->CCR3 = 0;
}

// Timer setup for delays
void setup_delay_timer() {
    // Enable clock for TIM6
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;

    // Configure TIM6 as a basic timer for delays
    TIM6->PSC = 7999;  // Prescaler to slow down the timer to 10 kHz (assuming 80 MHz clock)
    TIM6->ARR = 9999;  // Auto-reload value for generating delays
    TIM6->CR1 |= TIM_CR1_OPM;  // Enable one-pulse mode
}

void delay_ms(int ms) {
    // Configure TIM6 for a delay in milliseconds
    TIM6->CNT = 0;  // Reset the counter
    TIM6->PSC = 7999;  // Prescaler to slow down the timer to 10 kHz (assuming 80 MHz clock)
    TIM6->ARR = 10 * ms;  // Auto-reload value to generate the delay
    TIM6->EGR |= TIM_EGR_UG;  // Update generation to load the prescaler value
    TIM6->SR &= ~TIM_SR_UIF;  // Clear the update interrupt flag
    TIM6->CR1 |= TIM_CR1_CEN;  // Enable the counter

    // Wait until the update interrupt flag is set
    while (!(TIM6->SR & TIM_SR_UIF));
    TIM6->SR &= ~TIM_SR_UIF;  // Clear the update interrupt flag
}

void control_logic(float left_distance, float right_distance, float current_distance) {
    if (current_distance < 10.0) {
        motor_stop();
        delay(300);  // Delay for 300 ms
        motor_backward();  // Reverse the robot
        delay(1000);  // Move backward for a short duration

        // Check distances to decide the next move
        if (left_distance > right_distance) {
            motor_turn_left();
        } else {
            motor_turn_right();
        }
    } else {
        motor_forward();
    }
}
