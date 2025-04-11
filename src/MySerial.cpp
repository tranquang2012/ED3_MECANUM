#include "MySerial.h"

extern double w1, w1_ref, MOT1_cmd; // Reference and command for the motor
extern volatile long EncoderTick1;  // Encoder tick count for encoder 1
extern unsigned long Serial_time;   // Time for serial communication

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
