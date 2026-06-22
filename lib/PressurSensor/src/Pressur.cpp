#include <Wire.h>
#include "MS5837.h"
#include "Pressur.h"

MS5837 sensor;

bool PRESSUR::init() {
    Wire.begin();
    // https://github.com/bluerobotics/BlueRobotics_MS5837_Library
    // Initialize pressure sensor
    // Returns true if initialization was successful
    // We can't continue with the rest of the program unless we can initialize the sensor
    while (!sensor.init()) {
        Serial.println("Init failed!");
        Serial.println("Are SDA/SCL connected correctly?");
        Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
        Serial.println("\n\n\n");
        delay(5000);
    }
    
    sensor.setModel(MS5837::MS5837_02BA);
    sensor.setFluidDensity(1.2); 
    
    return true;
}

float getAltitude() {
    sensor.read();
    return sensor.altitude();
}

float getPressure() {
    sensor.read();
    return sensor.pressure();
}