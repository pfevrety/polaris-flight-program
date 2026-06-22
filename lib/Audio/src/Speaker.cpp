#include "Speaker.h"

void Speaker::begin(int volume) {
    if (!dfmp3.begin(/*isACK=*/true, /*doReset=*/true)) {  
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1. Please recheck the connection!"));
        Serial.println(F("2. Please insert the SD card!"));
        while(true); // blocage si échec
    }

    dfmp3.reset();
    dfmp3.setVolume(volume); // compris entre 0 et 30
}

// Joue le son correspondant à une clé d'événement
void Speaker::play(const char* event) {
    int track = lookup(event);
    if (track != -1) {
        dfmp3.advertise(track);
    } else {
        Serial.println("Événement inconnu !");
    }
}
