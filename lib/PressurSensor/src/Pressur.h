#pragma once
#ifndef RADIO_H
#define RADIO_H

#include <Arduino.h>
#include <SPI.h>

MS5837 sensor;

class PRESSUR {
public:
    PRESSUR();
    float getAltitude();
    float getPressure();
    bool init();

private:
};

#endif
