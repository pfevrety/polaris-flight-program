#pragma once
#ifndef SDLOGGER_H
#define SDLOGGER_H

#include <Arduino.h>
#include <SD.h>
#include "Acceleration.h"

#define SD_CS BUILTIN_SDCARD

class SDLogger {
public:
    bool begin();
    void logFlightData(const FlightPacket &fd);
    void close();

private:
    File logFile;
    bool ready = false;
    String filename;
};

#endif
