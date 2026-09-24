#include "Radio.h"

static volatile bool transmissionFlag = false;

void onTxDone() {
    transmissionFlag = true;
}

RADIO::RADIO() : lastTransmissionFinished(true) {}

bool RADIO::begin(int csPin, int resetPin, int irqPin) {
    Serial.println("Initializing LoRa...");
    Serial.print("CS: ");   Serial.println(csPin);
    Serial.print("RST: ");  Serial.println(resetPin);
    Serial.print("IRQ: ");  Serial.println(irqPin);

    LoRa.setPins(csPin, resetPin, irqPin);

    // Read RegVersion directly to distinguish an SPI wiring failure from radio configuration errors.
    pinMode(csPin, OUTPUT);
    digitalWrite(csPin, LOW);
    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(csPin, LOW);
    SPI.transfer(0x42);
    byte version = SPI.transfer(0x00);
    digitalWrite(csPin, HIGH);
    SPI.endTransaction();

    Serial.print("SPI RegVersion (doit etre 0x12): 0x");
    Serial.println(version, HEX);

    if (version != 0x12) {
        Serial.println("Module SPI non detecte - probleme hardware !");
        return false;
    }

    if (!LoRa.begin(868E6)) {
        Serial.println("Starting LoRa failed!");
        return false;
    }

    LoRa.onTxDone(onTxDone);
    Serial.println("LoRa Initializing OK!");
    return true;
}


void RADIO::sendFlightData(const FlightPacket &fd) {
    if (!lastTransmissionFinished) return;

    LoRa.beginPacket();
    LoRa.write((uint8_t*)&fd, sizeof(FlightPacket));
    LoRa.endPacket(true);

    lastTransmissionFinished = false;
}


void RADIO::update() {
    if (transmissionFlag) {
        transmissionFlag = false;
        lastTransmissionFinished = true;
        Serial.println("Transmission done");
    }
}
