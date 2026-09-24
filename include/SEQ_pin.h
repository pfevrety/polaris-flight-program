#pragma once
#include <Arduino.h>

/**
 * @file SEQ_pin.h
 * @brief Pinout mapping for the Fusex Polaris Sequencer Board (Teensy 4.1)
 * @details Handles flight state machine, launch detection jack, recovery servos,
 *          audio/visual status feedback, black-box SD logging, and TRIDGE communication.
 */

// --- Status & Feedback LEDs (WS2812B NeoPixel Strip) ---
constexpr uint8_t LED_INTERFACE       = 24;  ///< Data pin for addressable NeoPixels
constexpr uint8_t LED_COUNT           = 5;   ///< Number of status LEDs in the strip
constexpr uint8_t LED_DEBUG           = 13;  ///< Built-in Teensy LED (heartbeat blink)

// NeoPixel Strip Index Mapping
constexpr uint8_t LED_DECOLLAGE_INDEX = 0;  ///< Liftoff / Flight status
constexpr uint8_t LED_PORTE_INDEX     = 1;  ///< Parachute bay door servo status
constexpr uint8_t LED_CANSAT_INDEX    = 2;  ///< CanSat ejection bay servo status
constexpr uint8_t LED_IMU_INDEX       = 3;  ///< IMU sensor status
constexpr uint8_t LED_DEBUG_INDEX     = 4;  ///< General debug / sync status

// --- Sensors & Detection Inputs ---
constexpr uint8_t PIN_JACK            = 20;  ///< Umbilical pull-pin jack (Liftoff detection: LOW -> HIGH on release)
constexpr uint8_t PIN_IMU_1           = 4;   ///< CS pin for IMU 1 (SPI)
constexpr uint8_t PIN_IMU_2           = 5;   ///< CS pin for IMU 2 (SPI)

// --- Audio Feedback ---
constexpr uint8_t PIN_BUZZER          = 18;  ///< Active/piezo buzzer pin for flight sounds

// --- Actuator Control: Serial Bus Servos (ST3215 RS485/UART) ---
// Parachute Door Servo (Serial4: RX4=16, TX4=17)
constexpr uint8_t PIN_RX_SERVO_PARA   = 16;
constexpr uint8_t PIN_TX_SERVO_PARA   = 17;

// CanSat Door Servo (Serial3: RX3=15, TX3=14)
constexpr uint8_t PIN_RX_SERVO_CANSAT = 15;
constexpr uint8_t PIN_TX_SERVO_CANSAT = 14;

// Manual Arming Switches (Ground Safety Pad Check)
constexpr uint8_t SWITCH_DISARM       = 26;  ///< Switch input to disarm ejection servos
constexpr uint8_t SWITCH_ARM          = 27;  ///< Switch input to arm ejection servos (Priority over disarm)

// --- Inter-Board Bus (TRIDGE: Serial7) ---
constexpr uint8_t PIN_TRIDGE_RX       = 28;  ///< RX7
constexpr uint8_t PIN_TRIDGE_TX       = 29;  ///< TX7