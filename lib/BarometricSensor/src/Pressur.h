#ifndef PRESSUR_H
#define PRESSUR_H

#include <Arduino.h>

class PRESSUR {
public:
    PRESSUR(uint8_t pin);

    bool init();
    float getPressure();
    float getAltitude();

private:
    uint8_t _pin;

    static constexpr float V_REF     = 3.3f;
    static constexpr float ADC_MAX   = 1023.0f;

    static constexpr float V_MIN     = 0.40f;
    static constexpr float V_MAX     = 4.65f;
    static constexpr float P_MIN     = 10.0f;
    static constexpr float P_MAX     = 115.0f;

    // The divider keeps the sensor's 4.65 V maximum output below the ADC's 3.3 V limit.
    static constexpr float DIVIDER_RATIO = 0.6f;

    static constexpr float SEA_LEVEL_HPA = 1013.25f;
};

#endif
