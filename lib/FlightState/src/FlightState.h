#pragma once

// Recovery fallback only; the barometric apogee trigger is evaluated first.
constexpr uint32_t ASCENT_TIMEOUT_FALLBACK_MS = 17500;
constexpr uint32_t PARACHUTE_OPEN_DELAY_MS    = 2000;

constexpr uint32_t SYNC_BURST_COUNT         = 20;
constexpr uint32_t SYNC_BURST_INTERVAL_MS   = 50;


enum class FlightPhase : uint8_t {
    GROUND       = 0,
    ASCENDING    = 1,
    DESCENDING   = 2,
    LANDED       = 3,
    ERROR        = 4
};

enum class ServoArmedState : uint8_t {
    DISARMED = 0,
    ARMED    = 1
};


struct FlightStatus {
    FlightPhase     phase;
    ServoArmedState servoState;
    bool            parachuteDeployed;
    bool            hasLanded;
    uint32_t        flightStartTime;
};
