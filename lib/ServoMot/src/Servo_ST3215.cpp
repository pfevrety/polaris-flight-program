#include "Servo_ST3215.h"

Servo_ST3215::Servo_ST3215(HardwareSerialIMXRT &serial_connection, 
                             uint8_t id, 
                             int offset, 
                             int open_pos, 
                             int close_pos, 
                             uint16_t speed, 
                             uint8_t accel) 
{
    _id        = id;
    _offset    = offset;
    _open_pos  = open_pos;
    _close_pos = close_pos;
    _speed     = speed;
    _accel     = accel;
    st.pSerial = &serial_connection;
}

void Servo_ST3215::begin() {
    if (st.pSerial) {
        st.pSerial->begin(1000000); 
    }
}

void Servo_ST3215::open() {
    int target_pos = _open_pos + _offset;
    st.WritePosEx(_id, target_pos, _speed, _accel);
}

void Servo_ST3215::close() {
    int target_pos = _close_pos + _offset;
    st.WritePosEx(_id, target_pos, _speed, _accel);
}

int Servo_ST3215::getPosition() {
    return st.ReadPos(_id);
}

bool Servo_ST3215::isTargetReached(int targetPos, int tolerance) {
    int currentPos = getPosition();
    if (currentPos == -1) return false;
    return (abs(currentPos - targetPos) <= tolerance);
}