// https://github.com/finani/ICM42688
#include <ICM42688.h>
#include "Imu.h"
#include "Acceleration.h"
#include <Arduino.h>

// A modif
IMU::IMU(SPIClass &bus, uint8_t csPin) : icm(bus, csPin) {
}
bool IMU::init() {

    int status = icm.begin();

    if (status < 0) {
        Serial.println("Init failed!");
        Serial.println("Problème au niveau de l'initialisiation de l'IMU. Code d'erreur :");
        Serial.println("\n\n\n");
        Serial.println(status);
        return false;
    }    

    // configuration de la plage des mesures
    status = icm.setAccelFS(ICM42688::gpm2);
    if (status < 0) {
        Serial.println("Init failed!");
        Serial.println("Problème au niveau de l'initialisiation de l'IMU. Dans la partie de la borne sup de l'accélération Code d'erreur :");
        Serial.println("\n\n\n");
        Serial.println(status);
        return false;
    }    
    status = icm.setGyroFS(ICM42688::dps250);
    if (status < 0) {
        Serial.println("Init failed!");
        Serial.println("Problème au niveau de l'initialisiation de l'IMU. Dans la partie de la borne sup de l'accélération angulaire. Code d'erreur :");
        Serial.println("\n\n\n");
        Serial.println(status);
        return false;
    }    

    // Voir avec cris si on met ca
    // (optional) int enableDataReadyInterrupt() An interrupt is tied to the data output rate. The ICM42688 INT pin will issue a 50us pulse when data is ready. This is extremely useful for using interrupts to clock data collection that should occur at a regular interval. Please see the Interrupt_SPI example. This function enables this interrupt, which will occur at a frequency given by the SRD. This function returns a positive value on success and a negative value on failure. The following is an example of enabling the data ready interrupt.

    Serial.println("IMU initialisé avec succès");

    return true;}


bool IMU::calibrate() {
    int status = icm.calibrateGyro();

    if (status < 0) {
        Serial.println("Erreur lors de la calibration du gyro. Code d'erreur");
        Serial.println(status);
        return false;

    }
    // voir si on fait la calibration Golden Unit on calibre l'imu avant de la souder et ensuite on modif le code pour avoir des offset constants

    return true;
}
float IMU::temp()  { return icm.temp(); }

AccelData IMU::getAccel() {
    float ax = icm.accX();
    float ay = icm.accY();
    float az = icm.accZ();
    
    float gx = icm.gyrX();
    float gy = icm.gyrY();
    float gz = icm.gyrZ();

    return { millis(), {ax, ay, az}, {gx, gy, gz} };
}