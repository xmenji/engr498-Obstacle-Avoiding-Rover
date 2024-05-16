#include "stm32l476xx.h"
#include "dc_motor.h"

// GPIO Pin Definitions for Motor Control
#define IN1_PIN (1 << 0)  // PC0
#define IN2_PIN (1 << 1)  // PC1
#define IN3_PIN (1 << 2)  // PC2
#define IN4_PIN (1 << 3)  // PC3

// Timer setup for delays
void setup_timers() {
    // Enable clock for TIM6
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM6EN;

    // Configure TIM6 as a basic timer for delays
    TIM6->PSC = 7999; // Prescaler to slow down the timer to 10 kHz (assuming 80 MHz clock)
    TIM6->ARR = 9999; // Auto-reload value for generating delays
    TIM6->CR1 |= TIM_CR1_OPM; // Enable one-pulse mode
}

// Function to configure GPIO pins for the motor driver
void configure_motor_pins() {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;  // Enable Clock for GPIOC

    // Set PC0, PC1, PC2, PC3 as output
    GPIOC->MODER &= ~((0x3 << (0 * 2)) | (0x3 << (1 * 2)) | (0x3 << (2 * 2)) | (0x3 << (3 * 2)));  // Clear mode bits
    GPIOC->MODER |= (0x1 << (0 * 2)) | (0x1 << (1 * 2)) | (0x1 << (2 * 2)) | (0x1 << (3 * 2));  // Set mode to output
}

// Motor control functions using GPIO adjustments
void motor_stop() {
    GPIOC->ODR &= ~(IN1_PIN | IN2_PIN | IN3_PIN | IN4_PIN);  // Set IN1, IN2, IN3, IN4 to LOW
}

void motor_forward() {
    GPIOC->ODR |= IN1_PIN;  // Set IN1 to HIGH
    GPIOC->ODR &= ~IN2_PIN; // Set IN2 to LOW
    GPIOC->ODR |= IN3_PIN;  // Set IN3 to HIGH
    GPIOC->ODR &= ~IN4_PIN; // Set IN4 to LOW
}

void motor_backward() {
    GPIOC->ODR &= ~IN1_PIN; // Set IN1 to LOW
    GPIOC->ODR |= IN2_PIN;  // Set IN2 to HIGH
    GPIOC->ODR &= ~IN3_PIN; // Set IN3 to LOW
    GPIOC->ODR |= IN4_PIN;  // Set IN4 to HIGH
}

void motor_turn_left() {
    GPIOC->ODR |= IN1_PIN;  // Set IN1 to HIGH
    GPIOC->ODR &= ~IN2_PIN; // Set IN2 to LOW
    GPIOC->ODR &= ~IN3_PIN; // Set IN3 to LOW
    GPIOC->ODR |= IN4_PIN;  // Set IN4 to HIGH
}

void motor_turn_right() {
    GPIOC->ODR &= ~IN1_PIN; // Set IN1 to LOW
    GPIOC->ODR |= IN2_PIN;  // Set IN2 to HIGH
    GPIOC->ODR |= IN3_PIN;  // Set IN3 to HIGH
    GPIOC->ODR &= ~IN4_PIN; // Set IN4 to LOW
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
        delay_ms(300);  // Delay for 300 ms
        motor_backward();  // Reverse the robot
        delay_ms(1000);  // Move backward for a short duration

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
