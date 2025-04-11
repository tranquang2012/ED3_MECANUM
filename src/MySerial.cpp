#include "MySerial.h"

extern double w1, w1_ref, MOT1_cmd; // Reference and command for the motor 1 - defined in MySetup.h
extern volatile long EncoderTick1;  // Encoder tick count for encoder 1 - defined in MySetup.h
extern unsigned long Serial_time;   // Time for serial communication - defined in MySetup.h

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
        Serial.print(w1);
        Serial.print("\t");
        Serial.print(w1_ref);
        Serial.print("\t");
        Serial.print(MOT1_cmd);
        Serial.print("\t");
        Serial.print(EncoderTick1);
        Serial.println();
    }
}

// Function to write the data to the Serial Monitor
// Send q5 to rotate the motor 5rad/s (CCW)
// Send q-10 to rotate the motor -10rad/s (CW)
void SerialDataWrite() 
{
    static String received_chars;
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        received_chars += inChar;
        if (inChar == '\n')
        {
            switch (received_chars[0])
            {
            case 'q':               
                received_chars.remove(0, 1);
                w1_ref = received_chars.toFloat();
                break;
            default:
                break;
            }
            received_chars = "";
        }
    }
}
