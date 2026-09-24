#include <Arduino.h>
#include <SPI.h>
#include "Radio.h"
#include <array>
#include "EXP_pin.h"
#include "ExpVar.h"
#include "LedController.h"
#include "LedFeedback.h"
#include "Tridge.h"
#include "FlightProgram_RCE.h"
#include "Pressur.h"
#include "FlightState.h"
#include "StepperAero.h"

LedController ledSystem(LED_COUNT, LED_INTERFACE);
LedFeedback ledFeedback(ledSystem);
RADIO Radio;
TRIDGE Tridge;
ExpData expData;
FlightProgram flightProg;
FlightPacket flightPacket;
Stepper_NEMA17HS19 Aerofreins;

PRESSUR pressur(BAROMETRE);

void setup() {
    Serial.begin(9600);

    pinMode(LED_DEBUG, OUTPUT);
    digitalWrite(LED_DEBUG, HIGH);

    pinMode(PIN_JACK_CAMERA, INPUT);
    pinMode(PIN_CAMERA, OUTPUT);
    digitalWrite(PIN_CAMERA, LOW);

    Tridge.begin();
    flightProg.begin();
    ledSystem.begin();
    ledSystem.setBrightness(60);
    pressur.init();
    Aerofreins.begin();

    ledFeedback.showBoot();

    #ifdef COUPLE_WORKING
        Serial.println(F("Attente du signal de synchronisation du Séquenceur..."));

        while (!Tridge.receiveSync()) {
            ledSystem.update();
            delay(10);
        }

        Serial.println(F("Envoi de l'acquittement..."));
        for (uint8_t i = 0; i < SYNC_BURST_COUNT; i++) {
            Tridge.sendSync();
            delay(SYNC_BURST_INTERVAL_MS);
        }

        Serial.println(F("Synchronisation réussie !"));
        ledFeedback.showSyncDone();
    #endif

    delay(100);

    bool ok = Radio.begin(LORA_NSS, LORA_RESET, LORA_DIO0);
    if (!ok) {
        ledFeedback.showError(LED_GPS_INDEX);
        Serial.println(F("[ERREUR] Radio non initialisée !"));
    } else {
        Serial.println(F("Radio initialisée avec succès"));
    }

    Aerofreins.set_zero();
    Serial.println(F("Mise à zéro aérofreins..."));
    while (!Aerofreins.isAtZero()) {
        Aerofreins.update();
        delay(1);
    }
    Aerofreins.stop();
    Serial.println(F("Aérofreins à zéro"));

    ledFeedback.readyToFly();
    Serial.println(F("=== Expérience prête ==="));
}

void loop() {

    static uint32_t lastBlink = 0;
    if (millis() - lastBlink >= 500) {
        lastBlink = millis();
        digitalToggle(LED_DEBUG);
    }

    if (digitalRead(PIN_JACK_CAMERA) == 1) {
        digitalWrite(PIN_CAMERA, HIGH);
    }

    #ifndef COUPLE_WORKING
        static bool simTriggered = false;
        if (!simTriggered && millis() > 5000) {
            simTriggered = true;
            flightPacket.phase = FlightPhase::ASCENDING;
            Serial.println(F("[SIM] Passage en vol simulé"));
            ledFeedback.onFlightPhaseChanged(FlightPhase::ASCENDING);
        }
    #endif

    #ifdef COUPLE_WORKING
        if (Tridge.receiveFlightPacket(flightPacket)) {
            Tridge.printFlightPacket(flightPacket);
            flightProg.processNewData(flightPacket);
            ledFeedback.onFlightPhaseChanged(flightPacket.phase);
        }
    #endif

    static uint32_t lastExpSend = 0;
    if (millis() - lastExpSend >= 20) {
        lastExpSend = millis();
        expData.altitude = pressur.getAltitude();
        Tridge.sendExpData(expData);
    }

    flightProg.update(flightPacket, expData.altitude);
    ledFeedback.update();

    Radio.update();
    Radio.sendFlightData(flightPacket);
}
