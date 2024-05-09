#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include "stm32l476xx.h"

// Definitions for servo control PWM channels
#define SERVO_FRONT_LEFT_CHANNEL  1
#define SERVO_FRONT_RIGHT_CHANNEL 2
#define SERVO_BACK_LEFT_CHANNEL   3
#define SERVO_BACK_RIGHT_CHANNEL  4

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize all servos connected to the system.
 */
void initializeServos(void);

/**
 * @brief Set the angle of a specific servo.
 * 
 * @param channel The channel number of the servo.
 * @param angle The desired angle from 0 to 180 degrees.
 */
void setServoAngle(int channel, int angle);

/**
 * @brief Command the robot to turn left.
 */
void turnLeft(void);

/**
 * @brief Command the robot to turn right.
 */
void turnRight(void);

/**
 * @brief Command the robot to drive straight.
 */
void driveStraight(void);

/**
 * @brief Command the robot to perform a 180-degree turn.
 */
void turnAround(void);

/**
 * @brief Simulate obtaining distance from an ultrasonic sensor.
 * 
 * @param sensor_id The identifier for the sensor.
 * @return float The distance measured by the sensor in centimeters.
 */
float get_distance_from_sensor(int sensor_id);

#ifdef __cplusplus
}
#endif

#endif // SERVO_CONTROL_H
