#include "SdLogger.h"

bool SDLogger::begin() {
    if (!SD.begin(SD_CS)) {
        Serial.println("SD FAIL - carte absente ou non formatée");
        return false;
    }

    // Nom de fichier unique à chaque boot : vol_001.csv, vol_002.csv...
    int index = 0;
    do {
        filename = "vol_" + String(index++) + ".csv";
    } while (SD.exists(filename.c_str()));

    logFile = SD.open(filename.c_str(), FILE_WRITE);
    if (!logFile) {
        Serial.println("SD FAIL - impossible de créer le fichier");
        return false;
    }

    // En-tête CSV
    logFile.println("timestamp,acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z,parachute,atterri,flight_state");
    logFile.flush();

    Serial.print("SD OK - fichier: ");
    Serial.println(filename);
    ready = true;
    return true;
}

void SDLogger::logFlightData(const FlightData &fd, int flightState) {
    if (!ready) return;

    logFile.print(fd.sensors.t);   logFile.print(",");
    logFile.print(fd.sensors.acc.x);       logFile.print(",");
    logFile.print(fd.sensors.acc.y);       logFile.print(",");
    logFile.print(fd.sensors.acc.z);       logFile.print(",");
    logFile.print(fd.sensors.gyro.x);      logFile.print(",");
    logFile.print(fd.sensors.gyro.y);      logFile.print(",");
    logFile.print(fd.sensors.gyro.z);      logFile.print(",");
    logFile.print(fd.parachuteDeployed); logFile.print(",");
    logFile.print(fd.atterie);           logFile.print(",");
    logFile.println(flightState);

    // flush toutes les 50 lignes pour ne pas perdre de données
    static int count = 0;
    if (++count % 50 == 0) logFile.flush();
}

void SDLogger::close() {
    if (ready) {
        logFile.flush();
        logFile.close();
        ready = false;
        Serial.println("SD - fichier fermé");
    }
}