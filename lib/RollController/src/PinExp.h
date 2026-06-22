#ifndef PINSEQ_H
#define PINSEQ_H
#include "Arduino.h"

constexpr int LED_INTERFACE = 1;

constexpr int LED_DEBUG_INDEX = 4;
constexpr int LED_GPS_INDEX = 2;
constexpr int LED_AEROFREINS_INDEX = 1;
constexpr int LED_ROULIS_INDEX = 0;
constexpr int LED_PRESSION_INDEX = 3;

constexpr int SWITCH_AEROFREINS = 2;

constexpr int PIN_VANNE_GAUCHE = 3;
constexpr int PIN_VANNE_DROITE = 4;

constexpr int SWITCH_ROULIS = 3;

constexpr int DEBUG_VERT = 4;
constexpr int DEBUG_BLEU = 5;
constexpr int DEBUG_ROUGE = 6;

constexpr int SIGNAL_BUTEE = 10;

constexpr int LORA_MOSI = 11;
constexpr int LORA_MISO = 12;
constexpr int LORA_CLOCK = 13;
constexpr int LORA_RESET = 14;
constexpr int LORA_DIO0 = 15;
constexpr int LORA_NSS = 38;

constexpr int DIR = 31;

constexpr int STEP = 32;

constexpr int CAMERA = 23;

constexpr int BAROMETRE = 22;

constexpr int TX_GPS = 21;
constexpr int RX_GPS = 20;

constexpr int SCL_CODEUR_MAGNETIQUE = 19;
constexpr int SDA_CODEUR_MAGNETIQUE = 18;

constexpr int CAPTEUR_TENSION_PARACHUTE = 37;
constexpr int CAPTEUR_THERMIQUE = 36;

constexpr int TX_SEQ = 35;
constexpr int RX_SEQ = 34;

constexpr int PIN_STEP = 32;
constexpr int PIN_DIR = 31;

#endif