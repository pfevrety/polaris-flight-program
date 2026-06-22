#include <Arduino.h>
#include <SPI.h>
#include "Radio.h"
#include <array>
#include "PinExp.h"
#include "ExpVar.h"
#include "LedController.h"
#include "Tridge.h"
#include "FlightProgram_RCE.h" 

LedController ledSystem(LED_COUNT, LED_INTERFACE);
RADIO Radio;
TRIDGE Tridge;

FlightProgram flightProg;
FlightData flightData;

void setup() {
    Serial.begin(9600);
    Tridge.begin();
    flightProg.begin();
    ledSystem.begin();

    ledSystem.setBrightness(80);
    ledSystem.setGlobalColor(LedController::BLUE); 
    
    #ifdef COUPLE_WORKING
        Serial.println("Attente du signal de synchronisation du Séquenceur...");
        while (!Tridge.receiveSync()) {
            delay(10);
        }
        Serial.println("Synchronisation réussie ! Démarrage de l'expérience.");
        ledSystem.setGlobalColor(LedController::GREEN);
        delay(400);
        ledSystem.setGlobalColor(LedController::OFF);
        delay(200);
        ledSystem.setGlobalColor(LedController::GREEN);
        delay(200);
    #endif


    delay(100);

    bool ok = Radio.begin(LORA_NSS, LORA_RESET, LORA_DIO0);
    if (!ok) {
        ledSystem.setColor(LED_DEBUG_INDEX, LedController::RED);
    } else {
        ledSystem.setColor(LED_DEBUG_INDEX, LedController::GREEN);
        Serial.println("Radio initialisée avec succès");
    }

    ledSystem.setGlobalColor(LedController::BLUE);
    ledSystem.setColor(LED_DEBUG_INDEX, LedController::ORANGE);
}

void loop() {
    if (Tridge.receiveData(flightData)) {
        Tridge.printFlightData(flightData);
        flightProg.processNewData(flightData); 
    }

    flightProg.update(flightData);

    Radio.update();
    Radio.sendFlightData(flightData);
}