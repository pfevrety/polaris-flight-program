#ifndef EXPVAR_H
#define EXPVAR_H
constexpr int maxFlightDuration = 20000; // Durée max de vol en ms ici 10 s
constexpr int burstTime = 50; // en ms temps d'une impulsion
constexpr float angle_seuil =0.11861; // en degré
constexpr float Min_open_time_vannes = 0.05; // en sec
constexpr float LED_COUNT = 5;

constexpr float Kp = 1.5f;              
constexpr float Kd = 0.4f;              
constexpr float DEADZONE = 4.0f;        // zone morte globale en degrés

constexpr uint32_t MIN_BURST_TIME = 60; // temps ouv minimum en ms
constexpr uint32_t COOL_DOWN_TIME = 40; // temps de fermeture minimum en ms
#endif


