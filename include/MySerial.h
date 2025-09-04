#ifndef MY_SERIAL_H // Include guard to prevent multiple inclusions of this header file
#define MY_SERIAL_H

#include <Arduino.h>

// Forward declarations for movement functions
void moveForward(double speed);
void moveBackward(double speed);
void turnLeft(double speed);
void turnRight(double speed);
void spinLeft(double speed);
void spinRight(double speed);
void stopAllMotors();

void SerialBegin();     // Function to initialize the serial communication
void SerialDataPrint(); // Function to print the data to the Serial Monitor
void SerialDataWrite(); // Function to print the data to the Serial Monitor

#endif