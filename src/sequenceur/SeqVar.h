#ifndef EXPVAR_H
#define EXPVAR_H
constexpr int maxFlightDuration = 7000; // Durée max de vol en ms ici 10 s
constexpr int burstTime = 50; // en ms temps d'une impulsion
constexpr float angle_seuil = 15.0;


// prendre une puissance de 2 pour l'optimisation
constexpr int AccelBufferLen{20};
constexpr float InvBufferLen{1.0f / static_cast<float>(AccelBufferLen)};
constexpr float LED_COUNT = 5;

#endif


