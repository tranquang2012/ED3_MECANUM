#ifndef MY_ENCODER_H // Standard guard while writing a library to prevent multiple inclusions of this header file
#define MY_ENCODER_H

// ==============================================
// Include the necessary libraries
// ==============================================
#include <Arduino.h>      // Arduino library for basic functions
#include <ESP32Encoder.h> // ESP32Encoder library for handling encoders

// ==============================================
// Pin definitions for the encoders
// ==============================================
#define ENC1_A 23 // Pin used on ESP32 for the ENC1_A
#define ENC1_B 22 // Pin used on ESP32 for the ENC1_B

// ==============================================
// Variable definitions for the encoders
// ==============================================
volatile long EncoderTick1;       // Encoder tick count for encoder 1
double w1;                        // Angular velocity for encoder 1
Encoder encoder1(ENC1_A, ENC1_B); // Create an instance of the Encoder class

// ==============================================
// Encoder class definition
// ==============================================
class Encoder
{
public:
    Encoder(byte ENCA, byte ENCB); // Constructor for the Encoder class
    void begin();                    // Method to set the pin mode and enable the encoder
    long getCount();                 // Method to read the encoder value
    double getVelocity();            // Method to get the velocity of the encoder

private:
    byte _ENCA, _ENCB;                  // Pin used on ESP32 for the ENC_A and ENC_B
    int _ENC_RES = 330;                   // Encoder resolution
    unsigned long w_time;                 // Time variable used to calculate the velocity
    double th, th_prev, w, w_prev, w_raw; // Variables used to calculate the velocity
    double alpha = .90;                   // Filter coefficient
    ESP32Encoder _encoder;                // ESP32Encoder object to handle the encoder
};

#endif
