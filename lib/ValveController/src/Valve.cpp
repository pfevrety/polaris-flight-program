#include "Valve.h"

Valve::Valve() 
    : _pinLeft(0), _pinRight(0), _currentState(PushDirection::NONE) {
}

void Valve::begin(uint8_t pinLeft, uint8_t pinRight) {
    _pinLeft = pinLeft;
    _pinRight = pinRight;

    pinMode(_pinLeft, OUTPUT);
    pinMode(_pinRight, OUTPUT);
    stop();
}

void Valve::pushLeft() {
    digitalWrite(_pinRight, LOW); 
    digitalWrite(_pinLeft, HIGH);
    _currentState = PushDirection::LEFT;
}

void Valve::pushRight() {
    digitalWrite(_pinLeft, LOW);  
    digitalWrite(_pinRight, HIGH);
    _currentState = PushDirection::RIGHT;
}

void Valve::stop() {
    digitalWrite(_pinLeft, LOW);
    digitalWrite(_pinRight, LOW);
    _currentState = PushDirection::NONE;
}

PushDirection Valve::getState() const {
    return _currentState;
}