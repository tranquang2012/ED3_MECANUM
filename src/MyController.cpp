#include "MyController.h"

// PID PID_w1(&w1, &MOT1_cmd, &w1_ref, kp, ki, kd, DIRECT);
Controller::Controller(double *input, double *output, double *ref)
    : _PID(input, output, ref, _kp, _ki, _kd, DIRECT) // Initialize the PID object with input, output, and reference
{
    _input = input;
    _output = output;
    _ref = ref;
}

void Controller::begin()
{
    _PID.SetMode(AUTOMATIC);
    _PID.SetOutputLimits(-255, 255);
    _PID.SetSampleTime(10);
}
void Controller::compute()
{
    _PID.SetTunings(_kp, _ki, _kd);
    _PID.Compute();
}