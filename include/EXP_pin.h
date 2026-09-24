#pragma once
#include <Arduino.h>

/**
 * @file EXP_pin.h
 * @brief Pinout mapping for the Fusex Polaris Experiments & Payload Board (Teensy 4.1)
 * @details Handles Active Roll Control (cold-gas solenoid valves), Airbrakes Actuator (NEMA17 stepper),
 *          Infineon KP215F1701 analog pressure sensor, RFM95W LoRa 868MHz telemetry, and Camera triggering.
 */

// --- Status & Feedback LEDs (WS2812B NeoPixel Strip) ---
constexpr uint8_t LED_INTERFACE        = 1;   ///< Data pin for addressable NeoPixels
constexpr uint8_t LED_COUNT            = 5;   ///< Number of status LEDs in the strip
constexpr uint8_t LED_DEBUG            = 13;  ///< Built-in Teensy LED (heartbeat blink)

// NeoPixel Strip Index Mapping
constexpr uint8_t LED_ROULIS_INDEX     = 0;  ///< Roll control subsystem status
constexpr uint8_t LED_AEROFREINS_INDEX = 1;  ///< Airbrakes subsystem status
constexpr uint8_t LED_GPS_INDEX        = 2;  ///< GPS / Radio status
constexpr uint8_t LED_PRESSION_INDEX   = 3;  ///< Barometer / Altimeter sensor status
constexpr uint8_t LED_DEBUG_INDEX      = 4;  ///< General debug / sync status

// --- Active Roll Control: Solenoid Cold-Gas Valves ---
constexpr uint8_t PIN_VALVE_LEFT       = 26;  ///< Left valve MOSFET gate trigger
constexpr uint8_t PIN_VALVE_RIGHT      = 27;  ///< Right valve MOSFET gate trigger
constexpr uint8_t SWITCH_ROULIS        = 3;   ///< Manual arm/test switch for roll control

// --- Airbrakes Subsystem (NEMA-17 Stepper via A4988/TMC2209 driver) ---
constexpr uint8_t PIN_DIR              = 31;  ///< Direction control pin
constexpr uint8_t PIN_STEP             = 32;  ///< Step pulse pin
constexpr uint8_t SIGNAL_BUTEE         = 10;  ///< Limit switch / zero position sensor (active LOW)
constexpr uint8_t SWITCH_AEROFREINS    = 2;   ///< Manual test/enable switch for airbrakes

// --- Telemetry Subsystem (RFM95W 868 MHz LoRa Transceiver via SPI) ---
constexpr uint8_t LORA_NSS             = 38;  ///< Chip Select (CS) pin
constexpr uint8_t LORA_RESET           = 14;  ///< Hardware Reset pin
constexpr uint8_t LORA_DIO0            = 15;  ///< Interrupt Request (IRQ) pin
constexpr uint8_t LORA_MOSI            = 11;  ///< SPI MOSI
constexpr uint8_t LORA_MISO            = 12;  ///< SPI MISO
constexpr uint8_t LORA_CLOCK           = 13;  ///< SPI SCK

// --- Pressure Sensor (Infineon KP215F1701 analog altimeter) ---
constexpr uint8_t BAROMETRE            = 22;  ///< Analog input via calibrated voltage divider (0-3.3V)

// --- Camera Triggering ---
constexpr uint8_t PIN_JACK_CAMERA      = 40;  ///< Camera umbilical jack input
constexpr uint8_t PIN_CAMERA           = 23;  ///< Action cam trigger output

// --- Inter-Board Bus (TRIDGE: Serial8) ---
constexpr uint8_t PIN_TRIDGE_RX        = 34;  ///< RX8 (connected to TX7 of Sequencer)
constexpr uint8_t PIN_TRIDGE_TX        = 35;  ///< TX8 (connected to RX7 of Sequencer)

// --- Secondary Sensor Interfaces (Optional / Future expansion) ---
constexpr uint8_t TX_GPS               = 21;
constexpr uint8_t RX_GPS               = 20;
constexpr uint8_t SCL_CODEUR           = 19;
constexpr uint8_t SDA_CODEUR           = 18;
constexpr uint8_t CAPTEUR_TENSION      = 37;
constexpr uint8_t CAPTEUR_THERMIQUE    = 36;
