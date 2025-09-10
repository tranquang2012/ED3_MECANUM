
// ==============================================
// Pin definitions for the encoders (4 motors = 4 encoders)
// ==============================================


// need confirmation
#define ENC1_A 26 // D26 - Motor1_ENC1 (Driver1 - Rear Right)
#define ENC1_B 25 // D25 - Motor1_ENC2 (Driver1 - Rear Right)

#define ENC2_A 14 // D14 - Motor2_ENC1 (Driver2 - Front Right)
#define ENC2_B 27 // D27 - Motor2_ENC2 (Driver2 - Front Right)

#define ENC3_A 21 // D21 - Motor3_ENC1 (Driver3 - Rear Left)
#define ENC3_B 19 // D19 - Motor3_ENC2 (Driver3 - Rear Left)

#define ENC4_A 18 // D18 - Motor4_ENC1 (Driver4 - Front Left)
#define ENC4_B 5  // D5  - Motor4_ENC2 (Driver4 - Front Left)

// ==============================================
// Pin definitions for the motors (4 motors)
// ==============================================


// need confirmation
#define MOT1_A 33 // D33 - Driver1_LPWM (Motor 1 - Rear Right)
#define MOT1_B 32 // D32 - Driver1_RPWM (Motor 1 - Rear Right)

#define MOT2_A 12 // D12 - Driver2_LPWM (Motor 2 - Front Right)
#define MOT2_B 13 // D13 - Driver2_RPWM (Motor 2 - Front Right)

#define MOT3_A 23 // D23 - Driver3_LPWM (Motor 3 - Rear Left)
#define MOT3_B 22 // D22 - Driver3_RPWM (Motor 3 - Rear Left)

#define MOT4_A 16 // D16 - Driver4_LPWM (Motor 4 - Front Left)
#define MOT4_B 17 // D17 - Driver4_RPWM (Motor 4 - Front Left)


// ==============================================
// PWM Channel definitions for the motors (4 motors = 8 PWM channels)
// ==============================================
#define PWM1_A 0 // PWM Channel attached to MOT1_A
#define PWM1_B 1 // PWM Channel attached to MOT1_B
#define PWM2_A 2 // PWM Channel attached to MOT2_A
#define PWM2_B 3 // PWM Channel attached to MOT2_B
#define PWM3_A 4 // PWM Channel attached to MOT3_A
#define PWM3_B 5 // PWM Channel attached to MOT3_B
#define PWM4_A 6 // PWM Channel attached to MOT4_A
#define PWM4_B 7 // PWM Channel attached to MOT4_B

// ==============================================
// Control Mode Configuration
// ==============================================
// Uncomment ONE of the following modes:
// #define WEBSOCKET_CONTROL  // Enable WebSocket remote control via web interface
#define SERIAL_CONTROL     // Enable Serial command control (q3, q5, etc.)

// ==============================================
// Variables for 4 motors
// ==============================================
extern unsigned long Serial_time;

// Motor 1 (Front Left) variables
extern double w1, w1_ref, MOT1_cmd;   
extern volatile long EncoderTick1;    

// Motor 2 (Front Right) variables
extern double w2, w2_ref, MOT2_cmd;   
extern volatile long EncoderTick2;    

// Motor 3 (Rear Left) variables
extern double w3, w3_ref, MOT3_cmd;   
extern volatile long EncoderTick3;    

// Motor 4 (Rear Right) variables
extern double w4, w4_ref, MOT4_cmd;   
extern volatile long EncoderTick4;

// ==============================================
// High-level movement functions
// ==============================================
void setMotorSpeeds(double frontLeft, double frontRight, double rearLeft, double rearRight);
void moveForward(double speed);
void moveBackward(double speed);
void turnLeft(double speed);
void turnRight(double speed);
void spinLeft(double speed);
void spinRight(double speed);
void stopAllMotors();
