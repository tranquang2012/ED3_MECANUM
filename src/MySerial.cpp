#include "MyEncoder.h"    // Library for the encoder
#include "MyMotor.h"      // Library for the motor
#include "MyController.h" // Library for the controller
#include "MySerial.h"     // Library for serial communication
#include "MySetup.h"     // Configuration and pin definitions

// External variables for all 4 motors - defined in MySetup.h
// extern double w1, w1_ref, MOT1_cmd; 
// extern double w2, w2_ref, MOT2_cmd; 
// extern double w3, w3_ref, MOT3_cmd; 
// extern double w4, w4_ref, MOT4_cmd; 
// extern volatile long EncoderTick1, EncoderTick2, EncoderTick3, EncoderTick4;
// extern unsigned long Serial_time;

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
        // Enhanced format showing all 4 motors for Serial Control Mode
        Serial.print("M1:");
        Serial.print(w1);
        Serial.print("/");
        Serial.print(w1_ref);
        Serial.print("/");
        Serial.print(MOT1_cmd);
        Serial.print("\t");
        
        Serial.print("M2:");
        Serial.print(w2);
        Serial.print("/");
        Serial.print(w2_ref);
        Serial.print("/");
        Serial.print(MOT2_cmd);
        Serial.print("\t");
        
        Serial.print("M3:");
        Serial.print(w3);
        Serial.print("/");
        Serial.print(w3_ref);
        Serial.print("/");
        Serial.print(MOT3_cmd);
        Serial.print("\t");
        
        Serial.print("M4:");
        Serial.print(w4);
        Serial.print("/");
        Serial.print(w4_ref);
        Serial.print("/");
        Serial.print(MOT4_cmd);
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
            // Add debug message for received command
            Serial.println("DEBUG: Received command '" + String(command) + "' with speed " + String(speed));
            
            // Extended Serial Control Mode - Individual motor commands + movement commands
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
                Serial.println("Calling moveForward(" + String(speed > 0 ? speed : 3.0) + ")");
                moveForward(speed > 0 ? speed : 3.0);
                break;
            case 'b': // Backward movement
                Serial.println("Calling moveBackward(" + String(speed > 0 ? speed : 3.0) + ")");
                moveBackward(speed > 0 ? speed : 3.0);
                break;
            case 'l': // Turn left
                Serial.println("Calling turnLeft(" + String(speed > 0 ? speed : 3.0) + ")");
                turnLeft(speed > 0 ? speed : 3.0);
                break;
            case 'g': // Turn right
                Serial.println("Calling turnRight(" + String(speed > 0 ? speed : 3.0) + ")");
                turnRight(speed > 0 ? speed : 3.0);
                break;
            case 's': // Stop all motors
                Serial.println("Calling stopAllMotors()");
                stopAllMotors();
                break;
            default:
                Serial.println("Unknown command: '" + String(command) + "'");
                Serial.println("Commands: q/w/e/r(motors 1-4), f/b(move), l/g(turn), s(stop)");
                Serial.println("Example: q5 (motor1=5), f3 (forward at 3), s (stop)");
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
