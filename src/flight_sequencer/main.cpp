#include <Arduino.h>
#include "SEQ_pin.h"
#include "FlightState.h"
#include "LedController.h"
#include "LedFeedback.h"
#include "BuzzerManager.h"
#include "ApogeeDetector.h"
#include "Tridge.h"
#include "ServoMot.h"
#include "SdLogger.h"
#include "SeqVar.h"

LedController leds(LED_COUNT, LED_INTERFACE);
LedFeedback ledFeedback(leds);
BuzzerManager buzzer(PIN_BUZZER);
TRIDGE tridge;
SDLogger logger;
ExpData expData;

Servo_ST3215 servoPara(Serial4, 1, 532, 2048, 0);
Servo_ST3215 servoCanSat(Serial3, 1, 1154, -230, 0);

FlightStatus state = {
    FlightPhase::GROUND,
    ServoArmedState::ARMED,
    false,
    false,
    0};

bool lastArmState = true;
float initial_altitude;
ApogeeDetector apogeeDetector;

void transitionTo(FlightPhase newPhase);
void runFlightStateMachine();
void sendAndLog();
AccelData getFakeAccel();
bool checkApogee(float currentAltitude, float launchAltitude);

void setup()
{
    Serial.begin(9600);
    Serial.println(F("=== Démarrage système ==="));

    buzzer.begin();
    leds.begin();
    leds.setBrightness(60);
    ledFeedback.showBoot();
    tridge.begin();

    pinMode(LED_DEBUG, OUTPUT);
    digitalWrite(LED_DEBUG, HIGH);

    servoPara.begin();
    servoCanSat.begin();

    delay(100);
    servoCanSat.close();
    ledFeedback.onServoArmed(LED_CANSAT_INDEX);
    servoPara.close();
    ledFeedback.onServoArmed(LED_PORTE_INDEX);

    Serial.println(F("Servos initialisés (position fermée)"));

    if (!logger.begin())
    {
        Serial.println(F("[ERREUR] Carte SD non disponible !"));
        ledFeedback.showSdError();
        buzzer.error();
        delay(2000);
        buzzer.stop();
    }
    else
    {
        Serial.println(F("SD OK"));
        ledFeedback.showSdOk();
    }

#if COUPLE_WORKING
    bool syncConfirmed = false;
    const uint8_t MAX_SYNC_ATTEMPTS = 5;

    for (uint8_t attempt = 0; attempt < MAX_SYNC_ATTEMPTS && !syncConfirmed; attempt++)
    {
        Serial.print(F("[SYNC] Tentative "));
        Serial.print(attempt + 1);
        Serial.print(F("/"));
        Serial.println(MAX_SYNC_ATTEMPTS);

        for (uint8_t i = 0; i < SYNC_BURST_COUNT; i++)
        {
            tridge.sendSync();
            delay(SYNC_BURST_INTERVAL_MS);
        }

        uint32_t tStart = millis();
        while (millis() - tStart < 3000)
        {
            if (tridge.receiveSync())
            {
                syncConfirmed = true;
                Serial.println(F("[SYNC] Acquittement reçu — synchronisation OK !"));
                break;
            }
            buzzer.update();
            ledFeedback.update();
            delay(10);
        }

        if (!syncConfirmed)
        {
            Serial.println(F("[SYNC] Pas de réponse, nouvelle tentative..."));
            delay(500);
        }
    }

    if (!syncConfirmed)
    {
        Serial.println(F("[SYNC] ÉCHEC — démarrage sans acquittement"));
        buzzer.error();
        delay(1000);
        buzzer.stop();
        ledFeedback.showError(LED_DEBUG_INDEX);
    }
    else
    {
        while (Serial7.available()) Serial7.read();

        while (!tridge.receiveExpData(expData)) {
            buzzer.update();
            ledFeedback.update();
            delay(10);
        }

        initial_altitude = expData.altitude;
        Serial.print(F("[Pression] - Altitude initiale initialisée à : "));
        Serial.println(initial_altitude);

        buzzer.syncConfirm();
        ledFeedback.showSyncDone();
    }

#endif
    buzzer.bootConfirm();
    Serial.println(F("=== Système prêt ==="));
    pinMode(PIN_JACK, INPUT);

    ledFeedback.readyToFly();
}

void loop()
{
    static uint32_t lastBlink = 0;
    if (millis() - lastBlink >= 500) {
        lastBlink = millis();
        digitalToggle(13);
    }

    runFlightStateMachine();
    sendAndLog();
    ledFeedback.update();
    buzzer.update();

    if (tridge.receiveExpData(expData)) {
        Serial.print(F("Altitude expé : "));
        Serial.println(expData.altitude);
    }
}

void runFlightStateMachine()
{
    switch (state.phase)
    {

    case FlightPhase::GROUND:

        if (digitalRead(PIN_JACK) == 1)
        {
            state.flightStartTime = millis();
            transitionTo(FlightPhase::ASCENDING);
            buzzer.liftoff();
            Serial.println(F("[VOL] - Décollage détecté !"));
        }
        break;

    case FlightPhase::ASCENDING:
        // The sensor-confirmed apogee trigger always takes precedence over the time fallback.
        if (checkApogee(expData.altitude, initial_altitude))
        {
            transitionTo(FlightPhase::DESCENDING);
            Serial.println(F("[VOL] - Apogée détectée par baromètre !"));
        }
        // Keep a deterministic recovery path when altitude data is unavailable or invalid.
        else if (millis() - state.flightStartTime >= ASCENT_TIMEOUT_FALLBACK_MS)
        {
            transitionTo(FlightPhase::DESCENDING);
            Serial.println(F("[VOL] - Déploiement de secours (apogée non détectée à T+17,5 s)"));
        }
        break;

    case FlightPhase::DESCENDING:
    {
        static uint32_t cansatDeployTime = 0;
        static bool cansatReleased = false;

        if (!cansatReleased)
        {
            cansatReleased = true;
            cansatDeployTime = millis();
            buzzer.cansatDeploy();
            Serial.println(F("[DEPLOY] - Éjection CanSat (IPSA)"));
            servoCanSat.open();
            ledFeedback.onServoDisarmed(LED_CANSAT_INDEX);
        }

        if (!state.parachuteDeployed && (millis() - cansatDeployTime >= PARACHUTE_OPEN_DELAY_MS))
        {
            state.parachuteDeployed = true;
            buzzer.parachuteDeploy();
            Serial.println(F("[DEPLOY] - Ouverture Trappe Parachute"));
            servoPara.open();
            ledFeedback.onServoDisarmed(LED_PORTE_INDEX);
        }

        if (state.parachuteDeployed && (millis() - state.flightStartTime > 45000))
        {
            transitionTo(FlightPhase::LANDED);
        }
        break;
    }

    case FlightPhase::LANDED:
        if (!state.hasLanded)
        {
            state.hasLanded = true;
            buzzer.landed();
            logger.close();
            Serial.println(F("[VOL] - Atterrissage confirmé"));
        }
        break;

    case FlightPhase::ERROR:
        break;
    }
}

void sendAndLog()
{
    static uint32_t lastLogTime = 0;
    constexpr uint32_t LOG_INTERVAL_MS = 20;
    if (millis() - lastLogTime < LOG_INTERVAL_MS)
        return;
    lastLogTime = millis();

    FlightPacket packet;
    packet.sensors           = getFakeAccel();
    packet.phase             = state.phase;
    packet.parachuteDeployed = state.parachuteDeployed;
    packet.hasLanded         = state.hasLanded;

    tridge.sendFlightPacket(packet);

    if (state.phase != FlightPhase::LANDED && state.phase != FlightPhase::ERROR)
    {
        logger.logFlightData(packet);
    }
}

void transitionTo(FlightPhase newPhase)
{
    Serial.print(F("[STATE] - "));
    Serial.print(static_cast<int>(state.phase));
    Serial.print(F(" -> "));
    Serial.println(static_cast<int>(newPhase));

    state.phase = newPhase;
    ledFeedback.onFlightPhaseChanged(newPhase);
}

static float fakeAccCounter = 0.0f;

AccelData getFakeAccel()
{
    fakeAccCounter += 0.01f;
    return AccelData{
        (uint32_t)millis(),
        Accel(1.23f + fakeAccCounter, -4.56f, 9.81f),
        Accel(0.12f, 0.34f, -0.56f)};
}

/**
 * @brief Dynamic barometric apogee detection filter
 * @details Filters sensor noise using peak detection and consecutive altitude drop checks.
 *          Includes a burnout lockout timer to prevent false triggers during high dynamic pressure boost.
 */
bool checkApogee(float currentAltitude, float launchAltitude)
{
    return apogeeDetector.update(
        currentAltitude,
        launchAltitude,
        millis() - state.flightStartTime);
}
