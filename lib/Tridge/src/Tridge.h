#pragma once
#ifndef TRIDGE_H
#define TRIDGE_H

#include <Arduino.h>
#include "Acceleration.h"

/**
 * @brief Classe gérant la communication, la synchronisation et le traitement des données de vol.
 * * Cette classe permet d'établir une liaison série sécurisée (avec checksum) entre 
 * un séquenceur et des modules d'expérience, et offre des outils de débogage.
 */
class TRIDGE {
public:
    /**
     * @brief Constructeur par défaut de la classe TRIDGE
     */
    TRIDGE();

    /**
     * @brief Initialise les communications série selon le rôle du système
     * @return true si l'initialisation a réussi
     */
    bool begin();
    
    /**
     * @brief Envoie les données de vol avec une en-tête de synchronisation et un checksum
     * @param data Référence constante vers la structure de données de vol à envoyer
     */
    void sendData(const FlightData &data);

    /**
     * @brief Lit le port série pour recevoir et valider les données de vol
     * @param data Référence vers la structure où stocker les données reçues
     * @return true si les données ont été reçues avec succès (checksum valide), false sinon
     */
    bool receiveData(FlightData &data);

    /**
     * @brief Envoie une séquence d'octets spécifique pour la synchronisation
     */
    void sendSync();

    /**
     * @brief Écoute le port série à la recherche de la séquence de synchronisation attendue
     * @return true si la séquence de synchronisation exacte est détectée, false sinon
     */
    bool receiveSync();

    /**
     * @brief Affiche les données de vol formatées sur la console série
     * @param data Référence constante vers les données de vol à afficher
     */
    void printFlightData(const FlightData &data);

private:
    /**
     * @brief Calcule la somme de contrôle (checksum) d'un tableau d'octets
     * @param buffer Pointeur vers le tableau de données à analyser
     * @param length Taille des données en octets
     * @return La somme de contrôle calculée (opération XOR)
     */
    uint8_t calculateChecksum(const uint8_t* buffer, size_t length);

    /**
     * @brief Vitesse de communication partagée entre les différents rôles
     */
    uint8_t MutualBaudRate = 115200; 
};

#endif