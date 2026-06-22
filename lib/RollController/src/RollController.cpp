#include "RollController.h"

RollController::RollController() : roll_angle(0.0f) {}

void RollController::begin() {
    vannes.begin(PIN_VANNE_GAUCHE, PIN_VANNE_DROITE);
}

void RollController::resetAngle() {
    roll_angle = 0.0f;
}

void RollController::updateAngle(float gyroZ, float dt) {
    if (dt > 0.0f && dt < 0.5f) {
        roll_angle += gyroZ * dt;
    }
}

void RollController::control(float gyroZ, uint32_t &valveTimer) {
    if (millis() >= valveTimer) {
        if (vannes.getState() != PushDirection::NONE) {
            vannes.stop();
            valveTimer = millis() + COOL_DOWN_TIME;
        } else {
            float control_signal = (Kp * roll_angle) + (Kd * gyroZ);

            if (control_signal > DEADZONE) {
                vannes.pushLeft();
                valveTimer = millis() + MIN_BURST_TIME;
            } else if (control_signal < -DEADZONE) {
                vannes.pushRight();
                valveTimer = millis() + MIN_BURST_TIME;
            }
        }
    }
}

void RollController::forceLeft() {
    vannes.pushLeft();
}

void RollController::stopVannes() {
    if (vannes.getState() != PushDirection::NONE) {
        vannes.stop();
    }
}