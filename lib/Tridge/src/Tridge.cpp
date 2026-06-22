#include "Tridge.h"

TRIDGE::TRIDGE() {
}

uint8_t TRIDGE::calculateChecksum(const uint8_t* buffer, size_t length) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum ^= buffer[i]; // on utilise XOR
    }
    return checksum;
}

bool TRIDGE::begin() {
    #ifdef ROLE_SEQUENCEUR
        Serial7.begin(MutualBaudRate);
        Serial.println("Tridge sequenceur mode");
        return true;
    #else
        Serial8.begin(MutualBaudRate);
        Serial.println("Tridge expérience mode");
        return true;
    #endif
}

void TRIDGE::sendData(const FlightData &data) {
    #ifdef ROLE_SEQUENCEUR
        Serial.println("Envoi des données au séquenceur...");
        // Le séquenceur transmet sur Serial8
        Serial7.write(0xAA); 
        Serial7.write(0xBB); 

        const uint8_t* dataPtr = (const uint8_t*)&data;
        uint8_t checksum = calculateChecksum(dataPtr, sizeof(FlightData));
        Serial7.write((uint8_t*)&data, sizeof(FlightData));
        Serial7.write(checksum);
        Serial.println("Données envoyées au séquenceur.");
    #endif
}

bool TRIDGE::receiveData(FlightData &data) {
    while (Serial8.available() >= (2 + sizeof(FlightData) + 1)) {
        if (Serial8.read() == 0xAA) {
            if (Serial8.read() == 0xBB) {
                uint8_t* dataPtr = (uint8_t*)&data;
                Serial8.readBytes(dataPtr, sizeof(FlightData));
                
                uint8_t receivedChecksum = Serial8.read();
                uint8_t calculatedChecksum = calculateChecksum(dataPtr, sizeof(FlightData));
                
                if (receivedChecksum == calculatedChecksum) {
                    return true;
                } else {
                    #ifdef DEBUG
                    Serial.println("ERREUR : Checksum invalide.");
                    #endif
                    return false;
                }
            }
        }
    }
    return false;
}


void TRIDGE::printFlightData(const FlightData &data) {
    // mettre time
    Serial.print("Time: ");
    Serial.print(data.sensors.t);
    Serial.print(" ms | ");
    Serial.print("Accel: (");
    Serial.print(data.sensors.acc.x);
    Serial.print(", ");
    Serial.print(data.sensors.acc.y);
    Serial.print(", ");
    Serial.print(data.sensors.acc.z);
    Serial.print(") | Gyro: (");
    Serial.print(data.sensors.gyro.x);
    Serial.print(", ");
    Serial.print(data.sensors.gyro.y);
    Serial.print(", ");
    Serial.print(data.sensors.gyro.z);
    Serial.println(")");
    // mettre fligh state
    Serial.print("Flight State: ");
    switch (data.FLIGHT_STATE) {
        case 0: Serial.print("Au sol"); break;
        case 1: Serial.print("En vol"); break;
        case 2: Serial.print("Descente"); break;
        case 3: Serial.print("Atterri"); break;
        default: Serial.print("Inconnu"); break;
    };
    Serial.print("\n");
}

void TRIDGE::sendSync() {
    #ifdef ROLE_SEQUENCEUR
        Serial7.write(0x55);
        Serial7.write(0xAA);
        Serial7.write(0x99);
    #endif
}

bool TRIDGE::receiveSync() {
    #ifdef ROLE_EXPERIENCES
        while (Serial8.available() >= 3) {
            if (Serial8.read() == 0x55) {               
                if (Serial8.peek() == 0xAA) {           
                    Serial8.read();                     
                    if (Serial8.peek() == 0x99) {      
                        Serial8.read();                 
                        return true;                    
                    }
                }
            }
        }
    #endif
    return false;
}