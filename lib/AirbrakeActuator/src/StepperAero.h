#ifndef STEPPER_AERO_H
#define STEPPER_AERO_H

#include <Arduino.h>


class Stepper_NEMA17HS19
{
public:
    Stepper_NEMA17HS19(uint8_t stepPin = 32, uint8_t dirPin = 31, uint8_t zeroPin = 10);
    bool begin();

    void set_zero();
    bool isAtZero();

    void go_to_z(float z);
    void go_to_alpha(float alpha);

    void stop();
    void update();

    bool isRunning() const;
    int32_t getCurrentPos() const;
    int32_t getTargetPos() const;
    void setSpeed(uint32_t intervalMicros);
private:
    uint8_t  _stepPin;
    uint8_t  _dirPin;
    uint8_t  _zeroPin;

    int32_t  _posRequest;
    int32_t  _curPos;
    bool     _zeroing;
    bool     _zeroDone;
    bool     _atZero;
    bool     _stopped;

    uint32_t _lastStepTime;
    uint32_t _stepInterval;
    bool     _stepState;

    float    _pitch;

    void     _setDirection(bool towardZero);
    void     _doStep();
};

#endif
