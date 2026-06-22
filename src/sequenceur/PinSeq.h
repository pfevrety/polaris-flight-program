#ifndef PINSEQ_H
#define PINSEQ_H

constexpr int PIN_IMU_1 = 4;
constexpr int PIN_IMU_2 = 5;

constexpr int LED_DEBUG_INDEX = 4;
constexpr int LED_CANSAT_INDEX = 2;
constexpr int LED_PORTE_INDEX = 1;
constexpr int LED_DECOLLAGE_INDEX = 0;
constexpr int LED_IMU_INDEX = 3;


constexpr int PIN_BUZZER = 18;

constexpr int PIN_SERVO_PORTE = 5;

constexpr int PIN_SERVO_CANSAT = 5;

constexpr int PIN_JACK = 20;

SPIClass &SPI_IMU_1 = SPI1;
SPIClass &SPI_IMU_2 = SPI2;

constexpr int PIN_RX_SERVO_1 = 37;
constexpr int PIN_TX_SERVO_1 = 38;
constexpr int PIN_RX_SERVO_2 = 39;
constexpr int PIN_TX_SERVO_2 = 40;


// possible uniquement en phase de vol 0
constexpr int SWITCH_INTERF1 = 26; // Si on désarmer la fusée
constexpr int SWITCH_INTERF2 = 27; // Si on veut armer la fusée 27 a la priorité sur 26

constexpr int LED_INTERFACE = 24;

// mettre les pins de led et aussi les pins pour communiquer avec l'autre teensy

#endif