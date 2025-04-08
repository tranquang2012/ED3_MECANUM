#include <Arduino.h> // Arduino library for basic functions
// Custom Libraries: Header files (.h) contains pins, variables, and function declarations
// Sources (.cpp) contains function definitions

#include "MyEncoder.h"    // Library for the encoder
#include "MyMotor.h"      // Library for the motor
#include "MyController.h" // Library for the controller

double w1, w1_ref, MOT1_cmd; // Reference and command for the motor
volatile long EncoderTick1;       // Encoder tick count for encoder 1

Encoder encoder1(ENC1_A, ENC1_B); // Create an instance of the Encoder class
Motor motor1(MOT1_A, MOT1_B, PWM1_A, PWM1_B); // Create an instance of the Motor class
Controller controller1(&w1, &MOT1_cmd, &w1_ref); // Create an instance of the Controller class

//==============================================
// Arduino Setup
//==============================================
void setup()
{
  encoder1.begin();
  motor1.begin();
  controller1.begin();
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available() > 0) // Check if there is data available in the Serial Monitor
  {
    char command = Serial.read(); // Read the command from the Serial Monitor
    if (command == 's')           // If the command is 's'
    {
      w1_ref = 0;                 // Set the reference speed to 0
    }
    else if (command == 'f')      // If the command is 'f'
    {
      w1_ref = 100;               // Set the reference speed to 100
    }
    else if (command == 'b')      // If the command is 'b'
    {
      w1_ref = -100;              // Set the reference speed to -100
    }
  }
  EncoderTick1 = encoder1.getCount(); // Get the encoder count
  w1 = encoder1.getVelocity();        // Get the velocity from the encoder
  controller1.compute();              // Compute the PID control output
  motor1.send_pwm(MOT1_cmd);          // Send the PWM command to the motor
  Serial.println(w1);                 // Print the velocity to the Serial Monitor
}
