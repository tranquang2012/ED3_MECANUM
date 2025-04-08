#ifndef MY_MOTOR_H // Include guard to prevent multiple inclusions of this header file
#define MY_MOTOR_H

#include <Arduino.h>

// Pin definitions for the motors
#define MOT1_A 16 // Pin used on ESP32 for the MOT1_A
#define MOT1_B 4  // Pin used on ESP32 for the MOT1_B

// PWM Channel definitions for the motors
#define PWM1_A 0 // PWM Channel attached to MOT1_A
#define PWM1_B 1 // PWM Channel attached to MOT1_B

Motor motor1(MOT1_A, MOT1_B, PWM1_A, PWM1_B); // Create an instance of the Motor class

class Motor
{
public:
    Motor(byte MOTA, byte MOTB,
          int PWMA, int PWMB);       // Constructor to initialize the motor object
    void begin();                    // Method to set the pin mode and enable the motors
    void send_pwm(double motor_cmd); // Method to send the PWM signal to the motors

private:
    byte _MOTA, _MOTB;     // Pin used on ESP32 for the MOT_A
    int _PWMA, _PWMB;      // PWM Channel attached to MOT_A
    int _PWM_FREQ = 20000; // Frequency of the PWM signal
    int _PWM_RES = 8;      // Resolution of the PWM signal
};

#endif