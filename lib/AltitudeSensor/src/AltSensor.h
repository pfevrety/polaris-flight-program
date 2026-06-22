#ifndef ALTSENSOR_H
#define ALTSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <BaroSensor.h>

/**
 * @brief Classe de gestion d'un capteur barométrique pour mesurer l'altitude et la température
 *
 * Cette classe encapsule un capteur barométrique (BaroSensor) et fournit :
 *  - la lecture de l'altitude instantanée
 *  - la lecture d'altitude lissée sur plusieurs échantillons
 *  - la lecture de la température
 *  - la vérification de la connexion du capteur
 */
class AltSensor {
public:
    /**
     * @brief Constructeur de la classe AltSensor
     *
     * Initialise le capteur barométrique et prépare la lecture des mesures.
     */
    AltSensor();

    /**
     * @brief Récupère l'altitude instantanée en mètres
     * @return Altitude calculée à partir de la pression barométrique
     *         Retourne 0 si le capteur n'est pas opérationnel
     */
    int getAltitude();

    /**
     * @brief Récupère l'altitude moyenne sur plusieurs échantillons
     * @param samples Nombre d'échantillons à prendre (par défaut 10)
     * @return Altitude moyenne calculée
     */
    int getSmoothedAltitude(int samples = 10);

    /**
     * @brief Récupère la température mesurée par le capteur
     * @return Température en degrés Celsius
     */
    float getTemperature();

    /**
     * @brief Vérifie si le capteur barométrique est connecté et opérationnel
     * @return true si le capteur répond correctement, false sinon
     */
    bool isConnected();

private:
    static constexpr float seaLevelPressure = 1013.25 * 100.0; /**< Pression atmosphérique au niveau de la mer (Pa) */
    static constexpr float mu = 1.225;  /**< Masse volumique de l'air (kg/m^3) */
    static constexpr float g = 9.81;    /**< Accélération gravitationnelle (m/s^2) */
};

#endif // ALTSENSOR_H
