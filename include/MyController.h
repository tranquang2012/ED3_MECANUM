#ifndef MY_CONTROLLER_H // Include guard to prevent multiple inclusions of this header file
#define MY_CONTROLLER_H

#include <Arduino.h>
#include <PID_v1.h>

class Controller
{
public:
    // Controller(double *input, double *output, double *ref); // Constructor to initialize the PID controller
    Controller(double *, double *, double *); // Constructor to initialize the PID controller
    void begin();                             // Method to initialize the PID controller
    void compute();                           // Method to compute the PID control output

private:
    // double _kp = 20., _ki = 50., _kd = 0; // PID Parameters - to be tuned
    double _kp = 1., _ki = 3., _kd = 0; // PID Parameters - to be tuned
    double *_input, *_output, *_ref;         // Input variable for the PID controller
    int _direct = 0;
    PID _PID;
};

#endif