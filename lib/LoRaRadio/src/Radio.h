#pragma once
#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "Acceleration.h"

class RADIO {
public:
    RADIO();
    bool begin(int csPin, int resetPin, int irqPin);
    void sendFlightData(const FlightPacket &fd);
    void update();

private:
    volatile bool lastTransmissionFinished = true;
};

#endif
