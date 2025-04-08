#include "MyEncoder.h"

// ==============================================
// Constructor for the Encoder class
// ==============================================
Encoder::Encoder(byte ENCA, byte ENCB)
{
    _ENCA = ENCA; // From public ENC_A to private _ENC_A
    _ENCB = ENCB; // From public ENC_B to private _ENC_B
}

void Encoder::begin()
{
    _encoder.attachFullQuad(_ENCA, _ENCB);
    _encoder.setCount(0);
}

long Encoder::getCount()
{
    return _encoder.getCount();
}

double Encoder::getVelocity()
{
    th = _encoder.getCount() * 2 * PI / _ENC_RES; // Convert count to radians
    if (micros() - w_time >= 100 * 1e3)
    {
        w_raw = (th - th_prev) / ((micros() - w_time) * 1e-6); // Calculate the unfiltered velocity
        w = alpha * w_raw + (1 - alpha) * w;                   // Calculate the filtered velocity
        w_time = micros();
        th_prev = th;
    }
    return w;
}