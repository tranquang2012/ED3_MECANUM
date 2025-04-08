#include <Arduino.h>      // Arduino library for basic functions
// Custom Libraries: Header files (.h) contains pins, variables, and function declarations
// Sources (.cpp) contains function definitions

#include "MyEncoder.h"    // Library for the encoder
#include "MyMotor.h"      // Library for the motor
#include "MyController.h" // Library for the controller

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
  EncoderTick1 = encoder1.getCount();
  w1 = encoder1.getVelocity();
  controller1.compute();
  motor1.send_pwm(MOT1_cmd);
  Serial.println(w1);
}
