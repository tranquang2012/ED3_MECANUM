#include <Arduino.h> // Arduino library for basic functions

// The follwing libraries are custom libraries.
// Header files (.h) contains function declarations, and are located in the include folder
// Sources (.cpp) contains function definitions, and are located in the src folder
#include "MySetup.h"      // Pin and variable definitions
#include "MyEncoder.h"    // Library for the encoder
#include "MyMotor.h"      // Library for the motor
#include "MyController.h" // Library for the controller
#include "MySerial.h"     // Library for the controller

Encoder encoder1(ENC1_A, ENC1_B);                // Create an instance of the Encoder class
Motor motor1(MOT1_A, MOT1_B, PWM1_A, PWM1_B);    // Create an instance of the Motor class
Controller controller1(&w1, &MOT1_cmd, &w1_ref); // Create an instance of the Controller class

//==============================================
// Arduino Setup
//==============================================
void setup()
{
  encoder1.begin();    // Initialize the encoder
  motor1.begin();      // Initialize the motor
  controller1.begin(); // Initialize the controller
  SerialBegin();       // Initialize the serial communication
}

void loop()
{
  EncoderTick1 = encoder1.getCount(); // Get the encoder count
  w1 = encoder1.getVelocity();        // Get the velocity from the encoder
  controller1.compute();              // Compute the PID control output
  motor1.send_pwm(MOT1_cmd);          // Send the PWM command to the motor
  SerialDataPrint();                  // Print the data to the Serial Monitor
  SerialDataWrite();                  // Write the data to the Serial Monitor
}
