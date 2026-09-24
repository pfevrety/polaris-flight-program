#include "Pressur.h"

PRESSUR::PRESSUR(uint8_t pin) {
    _pin = pin;
}

bool PRESSUR::init() {
    pinMode(_pin, INPUT);
    analogRead(_pin);
    Serial.println("Capteur KP215F1701 initialisé.");
    return true;
}

float PRESSUR::getPressure() {
    int rawValue = analogRead(_pin);

    float voltageMeasured = V_REF * ((float)rawValue / ADC_MAX);

    // Undo the input-divider scaling before applying the sensor transfer function.
    float voltageSensor = voltageMeasured / DIVIDER_RATIO;

    voltageSensor = constrain(voltageSensor, V_MIN, V_MAX);

    // Inverse linear transfer function from the KP215F1701 datasheet.
    float pressure_kPa = P_MIN +
        ((voltageSensor - V_MIN) * (P_MAX - P_MIN)) / (V_MAX - V_MIN);

    return pressure_kPa * 10.0f;
}

float PRESSUR::getAltitude() {
    float pressure_hPa = getPressure();
    float altitude = 44330.0f * (1.0f - pow(pressure_hPa / SEA_LEVEL_HPA, 0.1903f));

    return altitude;
}
