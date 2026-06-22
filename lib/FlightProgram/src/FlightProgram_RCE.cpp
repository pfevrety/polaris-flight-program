#include "FlightProgram_RCE.h"

FlightProgram::FlightProgram() 
    : lastUpdateTime(0), valveTimer(0), flightStartTime(0), airbrakesOpened(false) {}

void FlightProgram::begin() {
    rollCtrl.begin();
    abCtrl.begin();
}

void FlightProgram::processNewData(const FlightData &flightData) {
    uint32_t currentTime = millis();
    
    if (lastUpdateTime != 0) {
        float dt = (currentTime - lastUpdateTime) / 1000.0f;
        rollCtrl.updateAngle(flightData.sensors.gyro.z, dt);
    }
    lastUpdateTime = currentTime;
}

void FlightProgram::update(FlightData &flightData) {

    if (flightData.FLIGHT_STATE == 0) {
        rollCtrl.resetAngle();
        airbrakesOpened = false;
        flightStartTime = 0;
    }

    else if (flightData.FLIGHT_STATE == 1) {
        if (flightStartTime == 0) {
            flightStartTime = millis();
            abCtrl.setSpeed(60);
        }

        uint32_t elapsed = millis() - flightStartTime;

        if (elapsed > maxFlightDuration) {
            flightData.FLIGHT_STATE = 2;
            flightData.parachuteDeployed = true;
            rollCtrl.stopVannes();
        } 
        else if (elapsed < 2000) { 
            rollCtrl.forceLeft();
        }
        else if (elapsed < 3000) {
            rollCtrl.stopVannes();
        }
        else {
            rollCtrl.stopVannes(); 
            
            if (!airbrakesOpened) {
                abCtrl.open(100); 
                airbrakesOpened = true; 
            }
        }
    }
    else {
        rollCtrl.stopVannes();
    }
}