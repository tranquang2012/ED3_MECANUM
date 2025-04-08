#ifndef MY_CONTROLLER_H // Include guard to prevent multiple inclusions of this header file
#define MY_CONTROLLER_H

#include <Arduino.h>
#include <PID_v1.h>

// Variable definitions for the PID controlle
double w1_ref, MOT1_cmd; // Reference and command for the motor
extern double w1;        // External variable for the motor speed

// PID PID_w1(&w1, &MOT1_cmd, &w1_ref, kp, ki, kd, DIRECT);
Controller controller1(&w1, &MOT1_cmd, &w1_ref); // Create an instance of the Controller class

class Controller
{
public:
    // Controller(double *input, double *output, double *ref); // Constructor to initialize the PID controller
    Controller(double *, double *, double *); // Constructor to initialize the PID controller
    void begin();                             // Method to initialize the PID controller
    void compute();                           // Method to compute the PID control output

private:
    double _kp = 20., _ki = 50., _kd = 0; // PID Parameters - to be tuned
    double *_input, *_output, *_ref;         // Input variable for the PID controller
    int _direct = 0;
    PID _PID;
};

#endif