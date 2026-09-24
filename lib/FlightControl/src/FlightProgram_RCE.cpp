#include "FlightProgram_RCE.h"
#include "StepperAero.h"

extern Stepper_NEMA17HS19 Aerofreins;

constexpr float TARGET_APOGEE_METERS     = 1000.0f;  ///< Target apogee for Polaris
constexpr uint32_t MOTOR_BURNOUT_TIME_MS = 2500;    ///< Cesaroni Pro98 burn time (~2.5s)
constexpr float MAX_AIRBRAKES_STROKE_MM  = 5.0f;     ///< Maximum airbrakes extension

FlightProgram::FlightProgram(FlightProgramMode mode)
    : mode(mode)
    , lastUpdateTime(0)
    , valveTimer(0)
    , flightStartTime(0)
    , airbrakesOpened(false)
    , lastAltitude(0.0f)
    , lastAltitudeTime(0)
    , verticalVelocity(0.0f)
{}

void FlightProgram::begin() {
    rollCtrl.begin();
    if (mode == FlightProgramMode::NOMINAL) {
        Serial.println(F("[FlightProgram] Mode NOMINAL activé (Boucle fermée Roulis + Aérofreins 1000m)"));
    } else {
        Serial.println(F("[FlightProgram] Mode BENCH_TEST activé (Séquence de banc d'essai)"));
    }
}

void FlightProgram::setMode(FlightProgramMode newMode) {
    mode = newMode;
}

void FlightProgram::processNewData(const FlightPacket &flightData) {
    uint32_t currentTime = millis();

    if (lastUpdateTime != 0) {
        float dt = (currentTime - lastUpdateTime) / 1000.0f;
        rollCtrl.updateAngle(flightData.sensors.gyro.z, dt);
    }
    lastUpdateTime = currentTime;
}

void FlightProgram::update(FlightPacket &flightData, float currentAltitude) {
    if (mode == FlightProgramMode::NOMINAL) {
        updateNominal(flightData, currentAltitude);
    } else {
        updateBenchTest(flightData);
    }
}

void FlightProgram::updateNominal(const FlightPacket &flightData, float currentAltitude) {
    if (flightData.phase == FlightPhase::GROUND) {
        rollCtrl.resetAngle();
        flightStartTime = 0;
        lastAltitude = currentAltitude;
        lastAltitudeTime = millis();
        verticalVelocity = 0.0f;
        airbrakesOpened = false;
        Aerofreins.go_to_z(0.0f);
    }
    else if (flightData.phase == FlightPhase::ASCENDING) {
        if (flightStartTime == 0) {
            flightStartTime = millis();
            Serial.println(F("[VOL NOMINAL] Démarrage séquence de vol"));
        }

        uint32_t elapsed = millis() - flightStartTime;

        rollCtrl.control(flightData.sensors.gyro.z, valveTimer);

        if (elapsed < MOTOR_BURNOUT_TIME_MS) {
            // Airbrakes remain retracted during boost to avoid adding drag under peak dynamic pressure.
            Aerofreins.go_to_z(0.0f);
        } else {
            regulateAirbrakes(currentAltitude);
        }

        Aerofreins.update();
    }
    else {
        rollCtrl.stopVannes();
        Aerofreins.go_to_z(0.0f);
        Aerofreins.stop();
    }
}

void FlightProgram::regulateAirbrakes(float currentAltitude) {
    uint32_t now = millis();
    if (lastAltitudeTime != 0 && (now - lastAltitudeTime) >= 100) {
        float dt = (now - lastAltitudeTime) / 1000.0f;
        float dz = currentAltitude - lastAltitude;
        verticalVelocity = dz / dt;

        lastAltitude = currentAltitude;
        lastAltitudeTime = now;

        // This deliberately conservative estimate excludes drag added by deployed airbrakes.
        if (verticalVelocity > 0.0f) {
            float projectedApogee = currentAltitude + (verticalVelocity * verticalVelocity) / (2.0f * 9.81f);

            if (projectedApogee > TARGET_APOGEE_METERS) {
                float excessAltitude = projectedApogee - TARGET_APOGEE_METERS;
                float stroke = constrain(excessAltitude * 0.02f, 0.0f, MAX_AIRBRAKES_STROKE_MM);
                Aerofreins.setSpeed(200);
                Aerofreins.go_to_z(stroke);
            } else {
                Aerofreins.setSpeed(200);
                Aerofreins.go_to_z(0.0f);
            }
        } else {
            Aerofreins.go_to_z(0.0f);
        }
    }
}

void FlightProgram::updateBenchTest(const FlightPacket &flightData) {
    if (flightData.phase == FlightPhase::GROUND) {
        rollCtrl.resetAngle();
        airbrakesOpened = false;
        flightStartTime = 0;
    }
    else if (flightData.phase == FlightPhase::ASCENDING) {
        if (flightStartTime == 0) {
            flightStartTime = millis();
            Serial.println(F("[BENCH TEST] Séquence de test démarrée"));
        }

        uint32_t elapsed = millis() - flightStartTime;

        if (elapsed < 3000) {
            Aerofreins.setSpeed(300);
            Aerofreins.go_to_z(2.5f);
        }
        else if (elapsed < 8000) {
            Aerofreins.setSpeed(200);
            Aerofreins.go_to_z(0.0f);
        }
        else if (elapsed < 10000) {
            Aerofreins.setSpeed(200);
            Aerofreins.go_to_z(5.0f);
        }
        else {
            Aerofreins.setSpeed(1000);
            Aerofreins.go_to_z(0.0f);
        }

        if (elapsed < 1000) {
            rollCtrl.forceLeft();
            Serial.println(F("[TEST ROULIS] Vanne Gauche"));
        }
        else if (elapsed < 2000) {
            rollCtrl.stopVannes();
            Serial.println(F("[TEST ROULIS] Stop"));
        }
        else if (elapsed < 3000) {
            rollCtrl.forceRight();
            Serial.println(F("[TEST ROULIS] Vanne Droite"));
        }
        else {
            rollCtrl.stopVannes();
        }

        Aerofreins.update();
    }
    else {
        rollCtrl.stopVannes();
        Aerofreins.stop();
    }
}
