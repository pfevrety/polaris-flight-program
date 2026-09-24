#include "LedController.h"

LedController::LedController(int ledCount, int ledPin)
    : pixels(ledCount, ledPin, NEO_GRB + NEO_KHZ800),
      ledCount(ledCount)
{
    blinkConfigs = new BlinkConfig[ledCount];
}


void LedController::begin() {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void LedController::setGlobalBlink(Color colorOn, uint32_t intervalMs, Color colorOff) {
    for (int i = 0; i < ledCount; i++) {
        setBlink(i, colorOn, intervalMs, colorOff);
    }
}

void LedController::stopGlobalBlink(Color finalColor) {
    for (int i = 0; i < ledCount; i++) {
        stopBlink(i, finalColor);
    }
}

void LedController::setBrightness(uint8_t brightness) {
    pixels.setBrightness(brightness);
}

void LedController::setColor(int index, Color color) {
    if (index < 0 || index >= ledCount) return;

    blinkConfigs[index].active = false;

    pixels.setPixelColor(index, getColorValue(color));
    pixels.show();
}

void LedController::setGlobalColor(Color color) {
    uint32_t c = getColorValue(color);
    for (int i = 0; i < ledCount; i++) {
        blinkConfigs[i].active = false;
        pixels.setPixelColor(i, c);
    }
    pixels.show();
}

void LedController::setBlink(int index, Color colorOn, uint32_t intervalMs, Color colorOff) {
    if (index < 0 || index >= ledCount) return;

    BlinkConfig& cfg = blinkConfigs[index];
    cfg.active       = true;
    cfg.colorOn      = colorOn;
    cfg.colorOff     = colorOff;
    cfg.intervalMs   = intervalMs;
    cfg.lastToggleMs = millis();
    cfg.state        = true;

    pixels.setPixelColor(index, getColorValue(colorOn));
    pixels.show();
}

void LedController::stopBlink(int index, Color finalColor) {
    if (index < 0 || index >= ledCount) return;

    blinkConfigs[index].active = false;
    pixels.setPixelColor(index, getColorValue(finalColor));
    pixels.show();
}

void LedController::update() {
    bool changed = false;
    uint32_t now = millis();

    for (int i = 0; i < ledCount; i++) {
        BlinkConfig& cfg = blinkConfigs[i];
        if (!cfg.active) continue;

        if (now - cfg.lastToggleMs >= cfg.intervalMs) {
            cfg.state        = !cfg.state;
            cfg.lastToggleMs = now;
            Color c = cfg.state ? cfg.colorOn : cfg.colorOff;
            pixels.setPixelColor(i, getColorValue(c));
            changed = true;
        }
    }

    if (changed) pixels.show();
}

void LedController::show() {
    pixels.show();
}


uint32_t LedController::getColorValue(Color color) const {
    switch (color) {
        case Color::GREEN:  return pixels.Color(0,   255, 0);
        case Color::RED:    return pixels.Color(255, 0,   0);
        case Color::BLUE:   return pixels.Color(0,   0,   255);
        case Color::ORANGE: return pixels.Color(255, 80,  0);
        case Color::WHITE:  return pixels.Color(255, 255, 255);
        case Color::PURPLE: return pixels.Color(150, 0,   150);
        case Color::YELLOW: return pixels.Color(255, 200, 0);
        case Color::OFF:
        default:            return pixels.Color(0,   0,   0);
    }
}
