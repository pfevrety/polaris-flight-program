#pragma once
#ifndef AIRBRAKES_CONTROLLER_H
#define AIRBRAKES_CONTROLLER_H

#include <Arduino.h>
#include <Stepper.h>
#include "PinExp.h"

class AirbrakesController {
public:
    AirbrakesController();
    void begin();
    void setSpeed(int speed);
    void stop();
    void open(int steps);
private:
    Stepper aerofreins;
    static const int STEPS_PER_REVOLUTION = 200; // À ajuster au besoin
};
#endif