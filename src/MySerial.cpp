#include "MySerial.h"

// External variables for all 4 motors - defined in MySetup.h
extern double w1, w1_ref, MOT1_cmd; 
extern double w2, w2_ref, MOT2_cmd; 
extern double w3, w3_ref, MOT3_cmd; 
extern double w4, w4_ref, MOT4_cmd; 
extern volatile long EncoderTick1, EncoderTick2, EncoderTick3, EncoderTick4;
extern unsigned long Serial_time;

void SerialBegin() // Function to initialize the serial communication
{
    Serial.begin(115200);
    while (!Serial)
        ;
}

void SerialDataPrint() // Function to print the data to the Serial Monitor
{
    if (micros() - Serial_time >= 10000)
    {
        Serial_time = micros();
        
#ifdef SERIAL_CONTROL
        // Original format for Serial Control Mode: -0.00   1.00    255.00  0
        Serial.print(w1);
        Serial.print("\t");
        Serial.print(w1_ref);
        Serial.print("\t");
        Serial.print(MOT1_cmd);
        Serial.print("\t");
        Serial.print(EncoderTick1);
        Serial.println();
#endif

#ifdef WEBSOCKET_CONTROL
        // Detailed format for WebSocket Mode showing all 4 motors
        Serial.print("M1: ");
        Serial.print(w1);
        Serial.print("/");
        Serial.print(w1_ref);
        Serial.print("/");
        Serial.print(MOT1_cmd);
        Serial.print("\t");
        
        Serial.print("M2: ");
        Serial.print(w2);
        Serial.print("/");
        Serial.print(w2_ref);
        Serial.print("/");
        Serial.print(MOT2_cmd);
        Serial.print("\t");
        
        Serial.print("M3: ");
        Serial.print(w3);
        Serial.print("/");
        Serial.print(w3_ref);
        Serial.print("/");
        Serial.print(MOT3_cmd);
        Serial.print("\t");
        
        Serial.print("M4: ");
        Serial.print(w4);
        Serial.print("/");
        Serial.print(w4_ref);
        Serial.print("/");
        Serial.print(MOT4_cmd);
        Serial.println();
#endif
    }
}

// Function to write the data to the Serial Monitor
// SERIAL_CONTROL mode: Simple commands (q5, q-10, etc.)
// WEBSOCKET_CONTROL mode: Full movement commands
void SerialDataWrite() 
{
    static String received_chars;
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        received_chars += inChar;
        if (inChar == '\n')
        {
            char command = received_chars[0];
            float speed = 0;
            if (received_chars.length() > 1) {
                speed = received_chars.substring(1).toFloat();
            }
            
#ifdef SERIAL_CONTROL
            // Simple Serial Control Mode - Original q3, q5 commands
            switch (command)
            {
            case 'q':               
                w1_ref = speed;
                Serial.println("Motor 1 set to: " + String(speed));
                break;
            default:
                Serial.println("Send q5 to rotate motor at 5rad/s, q-10 for -10rad/s");
                break;
            }
#endif

#ifdef WEBSOCKET_CONTROL
            // Full WebSocket Control Mode - All movement commands
            switch (command)
            {
            case 'q': // Motor 1 individual control              
                w1_ref = speed;
                Serial.println("Motor 1 set to: " + String(speed));
                break;
            case 'w': // Motor 2 individual control
                w2_ref = speed;
                Serial.println("Motor 2 set to: " + String(speed));
                break;
            case 'e': // Motor 3 individual control
                w3_ref = speed;
                Serial.println("Motor 3 set to: " + String(speed));
                break;
            case 'r': // Motor 4 individual control
                w4_ref = speed;
                Serial.println("Motor 4 set to: " + String(speed));
                break;
            case 'f': // Forward movement
                moveForward(speed);
                break;
            case 'b': // Backward movement
                moveBackward(speed);
                break;
            case 'l': // Turn left
                turnLeft(speed);
                break;
            case 'g': // Turn right
                turnRight(speed);
                break;
            case 's': // Stop all motors
                stopAllMotors();
                break;
            default:
                Serial.println("Commands: q/w/e/r(individual), f/b/l/g(movement), s(stop)");
                break;
            }
#endif
            received_chars = "";
        }
    }
}
