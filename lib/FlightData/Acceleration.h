#ifndef ACCELERATION_H
#define ACCELERATION_H

#include <Arduino.h>
#include "FlightState.h"

struct __attribute__((packed)) Accel {
    float x, y, z;
    Accel(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
    float getMagnitude()       const { return sqrt(x*x + y*y + z*z); }
    float getSquareMagnitude() const { return x*x + y*y + z*z; }
    void  update(float nx, float ny, float nz) { x=nx; y=ny; z=nz; }
};

struct __attribute__((packed)) AccelData {
    uint32_t t;
    Accel acc;
    Accel gyro;
};

struct __attribute__((packed)) FlightPacket {
    AccelData   sensors;
    FlightPhase phase;
    bool        parachuteDeployed;
    bool        hasLanded;
};

union PacketBuffer {
    FlightPacket data;
    uint8_t      bytes[sizeof(FlightPacket)];
};

#endif
