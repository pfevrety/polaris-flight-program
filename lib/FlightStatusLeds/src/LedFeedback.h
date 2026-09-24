#pragma once

#include "LedController.h"
#include "FlightState.h"

class LedFeedback {
public:
    LedFeedback(LedController& leds);

    void showBoot();
    void showSdOk();
    void showSdError();
    void showSyncDone();
    void readyToFly();

    void onServoArmed(int servo_pin);
    void onServoDisarmed(int servo_pin);
    void onFlightPhaseChanged(FlightPhase phase);
    void showError(int pin_index);

    void update();

private:
    LedController& leds;
    FlightPhase    currentPhase   = FlightPhase::GROUND;
    ServoArmedState servoState    = ServoArmedState::DISARMED;

    void applyPhaseColors();
};
