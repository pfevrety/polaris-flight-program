#pragma once

#include <Arduino.h>
#include "RollController.h"
#include "Tridge.h"
#include "ExpVar.h"
#include "FlightState.h"
#include "StepperAero.h"

/**
 * @enum FlightProgramMode
 * @brief Selects between nominal closed-loop guidance and bench test demonstration
 */
enum class FlightProgramMode {
    NOMINAL,      ///< Nominal flight: Closed-loop PD roll stabilization + active 1000m apogee regulation
    BENCH_TEST    ///< Ground/bench test: Scheduled open-loop actuator validation sequence
};

class FlightProgram {
public:
    explicit FlightProgram(FlightProgramMode mode = FlightProgramMode::NOMINAL);

    void begin();
    void setMode(FlightProgramMode newMode);
    FlightProgramMode getMode() const { return mode; }

    void processNewData(const FlightPacket &flightData);
    void update(FlightPacket &flightData, float currentAltitude = 0.0f);

private:
    void updateNominal(const FlightPacket &flightData, float currentAltitude);
    void updateBenchTest(const FlightPacket &flightData);
    void regulateAirbrakes(float currentAltitude);

    FlightProgramMode mode;
    RollController    rollCtrl;

    uint32_t lastUpdateTime;
    uint32_t valveTimer;
    uint32_t flightStartTime;
    bool     airbrakesOpened;

    // Guidance & state estimation for active airbrakes
    float    lastAltitude;
    uint32_t lastAltitudeTime;
    float    verticalVelocity;
};
