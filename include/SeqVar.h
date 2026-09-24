#pragma once
#include <Arduino.h>

/**
 * @file SeqVar.h
 * @brief Flight and sequence timing parameters for the Sequencer board
 */

constexpr uint32_t BURST_TIME_MS   = 50;     ///< Generic impulse time (ms)
constexpr float    ANGLE_SEUIL_DEG = 15.0f;  ///< Attitude threshold (deg)

constexpr size_t   AccelBufferLen  = 20;     ///< Buffer length for IMU sliding window
constexpr float    InvBufferLen    = 1.0f / static_cast<float>(AccelBufferLen);
