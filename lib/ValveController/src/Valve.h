#pragma once
#include <Arduino.h>

enum class PushDirection {
    NONE,
    LEFT,
    RIGHT
};

class Valve {
public:
    Valve();

    void begin(uint8_t pinLeft, uint8_t pinRight);

    void pushLeft();
    void pushRight();
    void stop();
    PushDirection getState() const;

private:
    uint8_t _pinLeft;
    uint8_t _pinRight;
    PushDirection _currentState;
};