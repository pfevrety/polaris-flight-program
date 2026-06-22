#include "AirbrakesController.h"

AirbrakesController::AirbrakesController() 
    : aerofreins(STEPS_PER_REVOLUTION, PIN_DIR, PIN_STEP) {}

void AirbrakesController::begin() {
    // Initialisations matérielles si nécessaires
}

void AirbrakesController::setSpeed(int speed) {
    aerofreins.setSpeed(speed);
}

void AirbrakesController::stop() {
    // Décommenter si ta librairie Stepper possède un stop ou une mise hors tension
    // aerofreins.stop(); 
}

void AirbrakesController::open(int steps) {
    aerofreins.step(steps);
}