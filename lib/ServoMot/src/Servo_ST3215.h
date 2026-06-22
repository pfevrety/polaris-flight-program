#pragma once
#include <Arduino.h>
#include <SCServo.h>

class Servo_ST3215 {
private:
    uint8_t   _id;
    int       _offset;
    int       _open_pos;
    int       _close_pos;
    uint16_t  _speed;
    uint8_t   _accel;
    
    SMS_STS   st;

public:
    Servo_ST3215(HardwareSerialIMXRT &serial_connection, 
                 uint8_t id, 
                 int offset, 
                 int open_pos, 
                 int close_pos, 
                 uint16_t speed = 1500, 
                 uint8_t accel = 50);

    void begin();
    void open();
    void close();
    int getPosition();
    bool isTargetReached(int targetPos, int tolerance = 15);
};