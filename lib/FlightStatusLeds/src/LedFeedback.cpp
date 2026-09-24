
#include "LedFeedback.h"
#if defined(ROLE_SEQUENCEUR)
#include "SEQ_pin.h"
#elif defined(ROLE_EXPERIENCE)
#include "EXP_pin.h"
#endif

LedFeedback::LedFeedback(LedController& leds) : leds(leds) {}

void LedFeedback::showBoot() {
    leds.setGlobalColor(LedController::Color::BLUE);
}

void LedFeedback::showError(int pin_index) {
    leds.setColor(pin_index, LedController::Color::RED);
}

void LedFeedback::showSdOk() {
#if defined(ROLE_SEQUENCEUR)
    leds.stopBlink(LED_DECOLLAGE_INDEX, LedController::Color::GREEN);
#endif
}

void LedFeedback::showSdError() {
#if defined(ROLE_SEQUENCEUR)
    leds.setBlink(LED_DECOLLAGE_INDEX, LedController::Color::RED, 300, LedController::Color::OFF);
#endif
}

void LedFeedback::showSyncDone() {
    leds.setColor(LED_DEBUG_INDEX, LedController::Color::GREEN);
}

void LedFeedback::readyToFly() {
#if defined(ROLE_SEQUENCEUR)
    leds.setBlink(LED_DECOLLAGE_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_DEBUG_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_IMU_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_PORTE_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_CANSAT_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
#elif defined(ROLE_EXPERIENCE)
    leds.setBlink(LED_ROULIS_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_AEROFREINS_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_GPS_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_PRESSION_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
    leds.setBlink(LED_DEBUG_INDEX, LedController::Color::GREEN, 700, LedController::Color::OFF);
#else
    for (int i = 0; i < 5; i++) {
        leds.setBlink(i, LedController::Color::GREEN, 700, LedController::Color::OFF);
    }
#endif
}

void LedFeedback::onServoArmed(int servo_pin) {
    leds.setColor(servo_pin, LedController::Color::GREEN);
}

void LedFeedback::onServoDisarmed(int servo_pin) {
    leds.stopBlink(servo_pin, LedController::Color::ORANGE);
}


void LedFeedback::onFlightPhaseChanged(FlightPhase phase) {
    currentPhase = phase;
    applyPhaseColors();
}

void LedFeedback::update() {
    leds.update();
}

void LedFeedback::applyPhaseColors() {
    switch (currentPhase) {
        case FlightPhase::GROUND:
#if defined(ROLE_SEQUENCEUR)
            leds.stopBlink(LED_DECOLLAGE_INDEX, LedController::Color::GREEN);
#elif defined(ROLE_EXPERIENCE)
            leds.setGlobalColor(LedController::Color::GREEN);
#endif
            break;

        case FlightPhase::ASCENDING:
            leds.setGlobalBlink(LedController::Color::RED, 200, LedController::Color::OFF);
            break;

        case FlightPhase::DESCENDING:
            leds.setGlobalBlink(LedController::Color::ORANGE, 600, LedController::Color::OFF);
            break;

        case FlightPhase::LANDED:
            leds.setGlobalColor(LedController::Color::GREEN);
            break;

        case FlightPhase::ERROR:
            for (int i = 0; i < 5; i++) {
                leds.setBlink(i, LedController::Color::RED, 100, LedController::Color::OFF);
            }
            break;
    }
}
