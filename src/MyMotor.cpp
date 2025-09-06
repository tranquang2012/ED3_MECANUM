#include "MyMotor.h"
#include "MySetup.h"

Motor::Motor(byte MOTA, byte MOTB, int PWMA, int PWMB)

{
    _MOTA = MOTA;
    _MOTB = MOTB;
    _PWMA = PWMA;
    _PWMB = PWMB;
}

void Motor::begin()
{
    // Configure the motor pins as output
    pinMode(_MOTA, OUTPUT);
    pinMode(_MOTB, OUTPUT);
    // Set up the PWM channels for the motors
    ledcSetup(_PWMA, _PWM_FREQ, _PWM_RES);
    ledcSetup(_PWMB, _PWM_FREQ, _PWM_RES);
    // Attach the PWM channels to the motor pins
    ledcAttachPin(_MOTA, _PWMA);
    ledcAttachPin(_MOTB, _PWMB);
}

void Motor::send_pwm(double motor_cmd)
{
#ifdef SERIAL_CONTROL
    // Only debug when motor_cmd is close to zero or changing significantly
    static double last_cmd = 999;
    if (abs(motor_cmd) < 0.1 || abs(motor_cmd - last_cmd) > 10) {
        Serial.print("PWM: ");
        Serial.print(motor_cmd);
        if (motor_cmd == 0) {
            Serial.println(" -> STOP: A=0, B=0");
        } else {
            Serial.println("");
        }
        last_cmd = motor_cmd;
    }
#endif
    
    // apply deadband, if command is very small, force it to zero for complete stop
    if (abs(motor_cmd) < 5.0) {
        motor_cmd = 0;
    }
    
    if (motor_cmd < 0)
    {
        ledcWrite(_PWMA, 0);
        ledcWrite(_PWMB, abs(motor_cmd));
    }
    else if (motor_cmd > 0)
    {
        ledcWrite(_PWMB, 0);
        ledcWrite(_PWMA, abs(motor_cmd));
    }
    else // motor_cmd == 0
    {
        // ensure complete stop by setting both channels to 0
        ledcWrite(_PWMA, 0);
        ledcWrite(_PWMB, 0);
    }
}