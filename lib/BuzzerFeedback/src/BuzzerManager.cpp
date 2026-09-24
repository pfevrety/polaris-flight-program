#include "BuzzerManager.h"

BuzzerManager::BuzzerManager(int pin) : pin(pin) {}

void BuzzerManager::begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}


void BuzzerManager::bootConfirm() {
    digitalWrite(pin, HIGH); delay(100);
    digitalWrite(pin, LOW);  delay(100);
    digitalWrite(pin, HIGH); delay(100);
    digitalWrite(pin, LOW);
}

void BuzzerManager::servoArmed() {
    stop();
    startPattern(200, 0, 1);
}

void BuzzerManager::servoDisarmed() {
    stop();
    startPattern(80, 80, 2);
}

void BuzzerManager::liftoff() {
    stop();
    startPattern(60, 60, 3);
}

void BuzzerManager::cansatDeploy() {
    stop();
    startPattern(200, 0, 1);
}

void BuzzerManager::parachuteDeploy() {
    stop();
    startPattern(800, 0, 1);
}

void BuzzerManager::syncConfirm() {
    stop();
    startPattern(25, 25, 20);
}

void BuzzerManager::landed() {
    stop();
    startPattern(150, 100, 4);
}

void BuzzerManager::error() {
    stop();
    startPattern(300, 700, -1);
}


void BuzzerManager::stop() {
    active   = false;
    buzzerOn = false;
    digitalWrite(pin, LOW);
}


void BuzzerManager::update() {
    if (!active) return;

    uint32_t now     = millis();
    uint32_t elapsed = now - lastToggle;

    if (buzzerOn && elapsed >= onDuration) {
        digitalWrite(pin, LOW);
        buzzerOn    = false;
        lastToggle  = now;
        repeatsDone++;

        if (repeatCount != -1 && repeatsDone >= repeatCount) {
            active = false;
        }
    }
    else if (!buzzerOn && elapsed >= offDuration && active) {
        if (repeatCount == -1 || repeatsDone < repeatCount) {
            digitalWrite(pin, HIGH);
            buzzerOn   = true;
            lastToggle = now;
        }
    }
}


void BuzzerManager::startPattern(uint32_t onMs, uint32_t offMs, int repeats) {
    onDuration  = onMs;
    offDuration = offMs;
    repeatCount = repeats;
    repeatsDone = 0;
    active      = true;
    buzzerOn    = true;
    lastToggle  = millis();
    digitalWrite(pin, HIGH);
}
