#ifndef GSM_H
#define GSM_H

#include <Arduino.h>
#include <Sim800L.h>
#include <SoftwareSerial.h>
#include <vector>

/**
 * @brief Numéro de téléphone par défaut pour l'envoi des SMS
 */
#define PHONE_NUMBER "0600000000"

/**
 * @brief Structure représentant une date et une heure
 */
struct Date {
    int day;    /**< Jour */
    int month;  /**< Mois */
    int year;   /**< Année */
    int hour;   /**< Heure */
    int minute; /**< Minute */
    int second; /**< Seconde */
};

/**
 * @brief Classe de gestion du module GSM SIM800L
 *
 * Cette classe permet :
 *  - l'initialisation du module GSM
 *  - l'envoi et la lecture de SMS
 *  - la récupération de l'heure depuis le module
 *  - la vérification de l'état du module
 */
class GSM {
public:
    /**
     * @brief Constructeur de la classe GSM
     *
     * N'initialise pas le module, il faut appeler begin() après création.
     */
    GSM();

    /**
     * @brief Initialise le module GSM et supprime les anciens SMS
     * @return true si le module est connecté et prêt, false sinon
     */
    bool begin();

    /**
     * @brief Vérifie que le module GSM est bien connecté à l'Arduino
     * @return true si le module répond, false sinon
     */
    bool testComponentConnection();

    /**
     * @brief Récupère l'heure et la date du module GSM
     * @return Structure Date avec l'heure actuelle du module
     */
    Date getTime();

    /**
     * @brief Envoie un SMS au numéro défini par PHONE_NUMBER
     * @param text Message à envoyer
     */
    void sendSMS(char* text);

    /**
     * @brief Récupère tous les nouveaux messages non lus
     * @return vecteur de String contenant le texte des SMS
     */
    std::vector<String> getNewMessages();

    /**
     * @brief Récupère la qualité du signal GSM
     * @return String représentant la qualité du signal
     */
    String getSignalQuality();

private:
    Sim800L sim800l; /**< Instance interne du module GSM */
    int8_t indexMessages = 0; /**< Index du dernier SMS lu */

    /**
     * @brief Compte le nombre total de SMS présents sur le module
     * @return Nombre de SMS
     */
    int countSms();
};

#endif
