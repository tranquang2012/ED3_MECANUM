
// ==============================================
// Pin definitions for the encoders (4 motors = 4 encoders)
// ==============================================
#define ENC1_A 26 // D26 - Motor1_ENC1 (Motor 1 - Front Left)
#define ENC1_B 25 // D25 - Motor1_ENC2 (Motor 1 - Front Left)
#define ENC2_A 27 // D27 - Motor2_ENC1 (Motor 2 - Front Right)
#define ENC2_B 14 // D14 - Motor2_ENC2 (Motor 2 - Front Right)
#define ENC3_A 19 // D19 - Motor3_ENC1 (Motor 3 - Rear Left)
#define ENC3_B 21 // D21 - Motor3_ENC2 (Motor 3 - Rear Left)
#define ENC4_A 18 // D18 - Motor4_ENC1 (Motor 4 - Rear Right)
#define ENC4_B 5  // D5  - Motor4_ENC2 (Motor 4 - Rear Right)

// ==============================================
// Pin definitions for the motors (4 motors)
// ==============================================
#define MOT1_A 33 // D32 - Driver1_RPWM (Motor 1 - Front Left)
#define MOT1_B 32 // D33 - Driver1_LPWM (Motor 1 - Front Left)
#define MOT2_A 12 // D12 - Driver2_LPWM (Motor 2 - Front Right)
#define MOT2_B 13 // D13 - Driver2_RPWM (Motor 2 - Front Right)
#define MOT3_A 16 // D16 - Driver4_LPWM (Motor 3 - Rear Left)
#define MOT3_B 17 // D17 - Driver4_RPWM (Motor 3 - Rear Left)
#define MOT4_A 22 // D22 - Driver3_LPWM (Motor 4 - Rear Right)
#define MOT4_B 23 // D23 - Driver3_RPWM (Motor 4 - Rear Right)

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
// Variables for 4 motors (extern declarations)
// ==============================================
extern unsigned long Serial_time; // Serial time in us

extern double w1, w1_ref, MOT1_cmd;   
extern volatile long EncoderTick1;    

extern double w2, w2_ref, MOT2_cmd;   
extern volatile long EncoderTick2;    

extern double w3, w3_ref, MOT3_cmd;   
extern volatile long EncoderTick3;    

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
