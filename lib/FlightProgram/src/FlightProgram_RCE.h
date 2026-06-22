#pragma once
#ifndef FLIGHT_PROGRAM_NOMINAL_H
#define FLIGHT_PROGRAM_NOMINAL_H

#include "RollController.h"
#include "AirbrakesController.h"
#include "Tridge.h"
#include "ExpVar.h"

class FlightProgram {
public:
    FlightProgram();
    void begin();
    void processNewData(const FlightData &flightData);
    void update(FlightData &flightData);

private:
    RollController rollCtrl;
    AirbrakesController abCtrl;

    uint32_t lastUpdateTime;
    uint32_t valveTimer;
    uint32_t flightStartTime;
    bool airbrakesOpened;
};

#endif