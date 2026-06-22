#pragma once
#ifndef IMU_H
#define IMU_H

#include "Acceleration.h"
#include <ICM42688.h>
#include <SPI.h>

// ICM42688FIFO(SPIClass &bus, uint8_t csPin, uint32_t SPI_HS_CLK=8000000) 

class IMU {
public:
    IMU(SPIClass &bus, uint8_t csPin);    bool init();

    float temp();
    
    AccelData getAccel();

    bool calibrate();

private:
    ICM42688 icm;
};

#endif
