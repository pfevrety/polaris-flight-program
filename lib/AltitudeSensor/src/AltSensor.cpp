#include "AltSensor.h"
#include <Arduino.h>

AltSensor::AltSensor() {
    BaroSensor.begin();
}

int AltSensor::getAltitude() {
    if (!BaroSensor.isOK()) {
        Serial.println("Erreur : capteur barométrique non opérationnel.");
        return 0;
    }

    float pressure = BaroSensor.getPressure() * 100.0; // hPa -> Pa
    // cf cours statique fluide avec formule sans variation de la masse volumique
    float altitude = (AltSensor::seaLevelPressure - pressure) / (AltSensor::mu * AltSensor::g);
    return (int)round(altitude);
}

int AltSensor::getSmoothedAltitude(int samples) {
    float sum = 0;
    for (int i = 0; i < samples; i++) {
        sum += getAltitude();
        delay(50);
    }
    return (int)round(sum / samples);
}

float AltSensor::getTemperature() {
    return BaroSensor.getTemperature();
}

bool AltSensor::isConnected() {
    bool ok = BaroSensor.isOK();
    if (!ok) {
        Serial.print("Capteur non detecte : ");
        Serial.println(BaroSensor.getError());
    }
    return ok;
}
