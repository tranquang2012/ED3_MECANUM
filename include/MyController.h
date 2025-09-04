#ifndef MY_CONTROLLER_H // Include guard to prevent multiple inclusions of this header file
#define MY_CONTROLLER_H

#include <Arduino.h> // Arduino library for basic functions
#include <PID_v1.h>  // PID library for PID control

class Controller
{
public:
    Controller(double *, double *, double *); // Constructor to initialize the PID controller
    void begin();                             // Method to initialize the PID controller
    void compute();                           // Method to compute the PID control output

private:
    double _kp = 5.0, _ki = 0.1, _kd = 0.5; // More conservative PID values for stability
    double *_input, *_output, *_ref;    // Input variable for the PID controller
    int _direct = 0;                    // Direction of the PID controller
    PID _PID;                           // PID object to handle the PID control
};

#endif