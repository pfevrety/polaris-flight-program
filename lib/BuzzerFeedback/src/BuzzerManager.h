#pragma once
#include <Arduino.h>

class BuzzerManager {
public:
    explicit BuzzerManager(int pin);

    void begin();

    void bootConfirm();
    void servoArmed();
    void servoDisarmed();
    void liftoff();
    void parachuteDeploy();
    void cansatDeploy();
    void landed();
    void error();
    void syncConfirm();

    void update();

    void stop();

private:
    int     pin;
    bool    active      = false;
    bool    buzzerOn    = false;
    uint32_t onDuration  = 0;
    uint32_t offDuration = 0;
    int      repeatCount = 0;
    int      repeatsDone = 0;
    uint32_t lastToggle  = 0;

    void startPattern(uint32_t onMs, uint32_t offMs, int repeats);
};
