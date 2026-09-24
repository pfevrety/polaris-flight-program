#pragma once
#include <Arduino.h>
#include "Acceleration.h"
#include "FlightState.h"

#define START_BYTE_1 0xAA
#define START_BYTE_2 0xBB

struct ExpData {
    float altitude;
};

class TRIDGE {
public:
    TRIDGE();
    bool begin();

    void sendFlightPacket(const FlightPacket &data);
    bool receiveFlightPacket(FlightPacket &data);

    void sendExpData(const ExpData &data);
    bool receiveExpData(ExpData &data);

    void printFlightPacket(const FlightPacket &data);

    void sendSync();
    bool receiveSync();

private:
    uint8_t calculateChecksum(const uint8_t* buffer, size_t length);
};
