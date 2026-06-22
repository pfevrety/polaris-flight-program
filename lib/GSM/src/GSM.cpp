#include "GSM.h" // https://registry.platformio.org/libraries/vittorioesposito/Sim800L%20Library%20Revised/installation
#include <vector>

// Attention : il faut désactiver le code PIN sur la carte PIN avec le tel

GSM::GSM() {
}

bool GSM::begin() {
    sim800l.begin();
    // Vérifie que le module est bien connecté
    if (! testComponentConnection()) {
        return false;
    }

    // Supprime tous les anciens messages correspondant à des test précédants
    if (sim800l.delAllSms()) { // Si true signifie qu'il y a une erreur
        return false;
    }
    
    return true;
}

// Vérifie que le système GSM est bien branché
// J'utilise getProductInfo puisque c'est la seule méthode
// qui ne dépend pas du réseau GSM
bool GSM::testComponentConnection (){
    String info = sim800l.getProductInfo();
    if (info.length() > 0) {
        return true;
    } else {
        return false;
}}

Date GSM::getTime() {
    int day, month, year, hour, minute, second;
    sim800l.RTCtime(&day, &month, &year, &hour, &minute, &second);
    
    Date date;
    date.day = day;
    date.month = month;
    date.year = year;
    date.hour = hour;
    date.minute = minute;
    date.second = second;

    return date;
}

void GSM::sendSMS(char* text) {
    sim800l.sendSms(PHONE_NUMBER, text);
}

// Regarde si il y a de nouveaux messages
std::vector<String> GSM::getNewMessages() {
    std::vector<String> newMessages;
    int totalSms = countSms();

    for (int i = indexMessages; i < totalSms; i++) {
        newMessages.push_back(sim800l.readSms(i)); // Faire une fonction pour filtrer les messages en fct de l'expéditeur
    }
    indexMessages = totalSms;
    return newMessages;
}

// Compte le nombre de sms
int GSM::countSms() {
    int count = 0;
    while (sim800l.getNumberSms(count) != "") {
        count++;
    }
    return count;
}

String GSM::getSignalQuality() {
    return sim800l.signalQuality();
}

