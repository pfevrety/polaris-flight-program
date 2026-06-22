#pragma once
#ifndef ROLL_CONTROLLER_H
#define ROLL_CONTROLLER_H

#include <Arduino.h>
#include "Valve.h"
#include "Tridge.h"
#include "ExpVar.h"
#include "PinExp.h"

class RollController {
public:
    RollController();
    void begin();
    void resetAngle();
    void updateAngle(float gyroZ, float dt);
    void control(float gyroZ, uint32_t &valveTimer);
    void stopVannes();
    void forceLeft();

private:
    Valve vannes;
    float roll_angle;
};

#endif