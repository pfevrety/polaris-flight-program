#pragma once
#include <Arduino.h>

/**
 * @file ExpVar.h
 * @brief Flight and actuator control parameters for the Experiments board
 */

constexpr uint32_t MAX_EXP_FLIGHT_DURATION_MS = 20000; ///< Maximum flight duration (safety timeout)
constexpr uint32_t BURST_TIME_MS              = 50;    ///< Solenoid valve burst time (ms)
constexpr float    ANGLE_SEUIL_DEG            = 0.11861f;
constexpr float    MIN_OPEN_TIME_VANNES_S     = 0.05f;

// --- Active Roll Control: PD Loop Constants ---
constexpr float    Kp                         = 1.5f;   ///< Proportional gain
constexpr float    Kd                         = 0.4f;   ///< Derivative gain
constexpr float    DEADZONE                   = 4.0f;   ///< Deadzone in degrees (hysteresis)

constexpr uint32_t MIN_BURST_TIME             = 60;     ///< Minimum open duration for gas valves (ms)
constexpr uint32_t COOL_DOWN_TIME             = 40;     ///< Minimum cooldown closed duration (ms)
