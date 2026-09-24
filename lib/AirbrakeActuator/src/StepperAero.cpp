#include "StepperAero.h"

Stepper_NEMA17HS19::Stepper_NEMA17HS19(uint8_t stepPin, uint8_t dirPin, uint8_t zeroPin)
    : _stepPin(stepPin)
    , _dirPin(dirPin)
    , _zeroPin(zeroPin)
    , _posRequest(0)
    , _curPos(0)
    , _zeroing(false)
    , _zeroDone(false)
    , _atZero(false)
    , _stopped(true)
    , _lastStepTime(0)
    , _stepInterval(1000)
    , _stepState(false)
    , _pitch(5.0f)
{}

bool Stepper_NEMA17HS19::begin()
{
    pinMode(_stepPin, OUTPUT);
    pinMode(_dirPin,  OUTPUT);
    pinMode(_zeroPin, INPUT_PULLUP);

    digitalWrite(_stepPin, LOW);
    digitalWrite(_dirPin,  LOW);

    Serial.println(F("[Stepper] Initialisé"));
    return true;
}


void Stepper_NEMA17HS19::set_zero()
{
    _zeroing  = true;
    _zeroDone = false;
    _stopped  = false;
    _setDirection(true);
    Serial.println(F("[Stepper] Retour zéro lancé"));
}

void Stepper_NEMA17HS19::stop()
{
    _stopped  = true;
    _zeroing  = false;
    Serial.println(F("[Stepper] Arrêt"));
}

void Stepper_NEMA17HS19::go_to_z(float z)
{
    _posRequest = (int32_t)(4096.0f * z / _pitch);
    _stopped    = false;

    _setDirection(_posRequest < _curPos);

    Serial.print(F("[Stepper] Cible : "));
    Serial.print(_posRequest);
    Serial.println(F(" steps"));
}

void Stepper_NEMA17HS19::go_to_alpha(float alpha)
{
    (void)alpha;
}


void Stepper_NEMA17HS19::update()
{
    if (_stopped) return;

    uint32_t now = micros();
    if (now - _lastStepTime < _stepInterval) return;
    _lastStepTime = now;

    if (_zeroing)
    {
        if (isAtZero())
        {
            _curPos   = 0;
            _zeroing  = false;
            _zeroDone = true;
            _stopped  = true;
            Serial.println(F("[Stepper] Zéro atteint"));
            return;
        }
        _doStep();
        return;
    }

    int32_t delta = _posRequest - _curPos;

    if (delta == 0)
    {
        _stopped = true;
        Serial.println(F("[Stepper] Position atteinte"));
        return;
    }

    _setDirection(delta < 0);
    _doStep();

    _curPos += (delta > 0) ? 1 : -1;
}

bool Stepper_NEMA17HS19::isAtZero()
{
    _atZero = (digitalRead(_zeroPin) == LOW);
    return _atZero;
}


bool Stepper_NEMA17HS19::isRunning() const
{
    return !_stopped;
}

int32_t Stepper_NEMA17HS19::getCurrentPos() const
{
    return _curPos;
}

int32_t Stepper_NEMA17HS19::getTargetPos() const
{
    return _posRequest;
}

void Stepper_NEMA17HS19::_setDirection(bool towardZero)
{
    bool dirState = towardZero ? LOW : HIGH;
    if (digitalRead(_dirPin) != dirState)
        digitalWrite(_dirPin, dirState);
}

void Stepper_NEMA17HS19::_doStep()
{
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_stepPin, LOW);
}
void Stepper_NEMA17HS19::setSpeed(uint32_t intervalMicros) {
    _stepInterval = intervalMicros;
}
