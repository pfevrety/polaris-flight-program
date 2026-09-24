#include "Tridge.h"

TRIDGE::TRIDGE() {}

uint8_t TRIDGE::calculateChecksum(const uint8_t *buffer, size_t length)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; i++)
    {
        checksum ^= buffer[i];
    }
    return checksum;
}

bool TRIDGE::begin()
{
    constexpr uint32_t baudRate = 115200;
#ifdef ROLE_SEQUENCEUR
    Serial7.begin(baudRate);
    Serial.println(F("[Tridge] - mode SEQUENCEUR initialisé (115200 baud)"));
#else
    Serial8.begin(baudRate);
    Serial.println(F("[Tridge] - mode EXPERIENCE initialisé (115200 baud)"));
#endif
    return true;
}

void TRIDGE::sendFlightPacket(const FlightPacket &data)
{
#ifdef ROLE_SEQUENCEUR
    if (Serial7.availableForWrite() < (int)(2 + sizeof(FlightPacket) + 1))
        return;

    Serial7.write(START_BYTE_1);
    Serial7.write(START_BYTE_2);

    const uint8_t *ptr = (const uint8_t *)&data;
    Serial7.write(ptr, sizeof(FlightPacket));

    uint8_t checksum = calculateChecksum(ptr, sizeof(FlightPacket));
    Serial7.write(checksum);
#endif
}

bool TRIDGE::receiveFlightPacket(FlightPacket &data)
{
#ifdef ROLE_EXPERIENCE
    while (Serial8.available() >= (int)(2 + sizeof(FlightPacket) + 1))
    {
        if (Serial8.peek() == START_BYTE_1)
        {
            Serial8.read();
            if (Serial8.peek() == START_BYTE_2)
            {
                Serial8.read();

                uint8_t *ptr = (uint8_t *)&data;
                Serial8.readBytes(ptr, sizeof(FlightPacket));

                uint8_t received = Serial8.read();
                uint8_t calc = calculateChecksum(ptr, sizeof(FlightPacket));

                if (received == calc)
                {
                    return true;
                }
                else
                {
                    Serial.println(F("[Tridge] Erreur Checksum FlightPacket"));
                    return false;
                }
            }
        }
        else
        {
            Serial8.read();
        }
    }
#endif
    return false;
}

void TRIDGE::sendExpData(const ExpData &data)
{
#ifdef ROLE_EXPERIENCE

    if (Serial8.availableForWrite() < (int)(2 + sizeof(ExpData) + 1))
        return;
    Serial8.write(START_BYTE_1);
    Serial8.write(START_BYTE_2);

    const uint8_t *ptr = (const uint8_t *)&data;
    Serial8.write(ptr, sizeof(ExpData));
    uint8_t checksum = calculateChecksum(ptr, sizeof(ExpData));
    Serial8.write(checksum);

#endif
}

bool TRIDGE::receiveExpData(ExpData &data)
{
#ifdef ROLE_SEQUENCEUR
    while (Serial7.available() >= (int)(2 + sizeof(ExpData) + 1))
    {

        if (Serial7.peek() == START_BYTE_1)
        {
            Serial7.read();

            if (Serial7.peek() == START_BYTE_2)
            {
                Serial7.read();

                uint8_t *ptr = (uint8_t *)&data;
                Serial7.readBytes(ptr, sizeof(ExpData));

                uint8_t received = Serial7.read();
                uint8_t calc = calculateChecksum(ptr, sizeof(ExpData));

                if (received == calc)
                {
                    return true;
                }
                else
                {
                    Serial.println(F("[Tridge EXP] Erreur - Checksum"));
                    return false;
                }
            }
        }
        else
        {
            Serial7.read();
        }
    }
#endif
    return false;
}
void TRIDGE::printFlightPacket(const FlightPacket &data)
{
    Serial.print("T: ");
    Serial.print(data.sensors.t);

    Serial.print(" | Acc: ");
    Serial.print(data.sensors.acc.x);
    Serial.print(",");
    Serial.print(data.sensors.acc.y);
    Serial.print(",");
    Serial.print(data.sensors.acc.z);
    Serial.print(" | Gyro: ");
    Serial.print(data.sensors.gyro.x);
    Serial.print(",");
    Serial.print(data.sensors.gyro.y);
    Serial.print(",");
    Serial.print(data.sensors.gyro.z);

    Serial.print(" | State: ");
    Serial.println(static_cast<uint8_t>(data.phase));
}

void TRIDGE::sendSync()
{
#ifdef ROLE_SEQUENCEUR
    Serial7.write(0x55);
    Serial7.write(0xAA);
    Serial7.write(0x99);
#else
    Serial8.write(0x55);
    Serial8.write(0xAA);
    Serial8.write(0x99);

#endif
}

bool TRIDGE::receiveSync()
{
#ifdef ROLE_EXPERIENCE
    while (Serial8.available() >= 3)
    {
        if (Serial8.read() == 0x55)
        {
            if (Serial8.peek() == 0xAA)
            {
                Serial8.read();
                if (Serial8.peek() == 0x99)
                {
                    Serial8.read();
                    sendSync();
                    return true;
                }
            }
        }
    }
#else
    while (Serial7.available() >= 3)
    {
        if (Serial7.read() == 0x55)
        {
            if (Serial7.peek() == 0xAA)
            {
                Serial7.read();
                if (Serial7.peek() == 0x99)
                {
                    return true;
                }
            }
        }
    }

#endif
    return false;
}
