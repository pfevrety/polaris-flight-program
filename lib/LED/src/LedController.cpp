#include "LedController.h"
#include <Adafruit_NeoPixel.h>

LedController::LedController(int LED_COUNT, int LED_PIN) 
  : pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800) {
    NumberOfLEDs = LED_COUNT;
}

uint32_t LedController::getColorValue(ColorName colorName) {
    switch (colorName) {
        case ColorName::GREEN:  return pixels.Color(0, 255, 0);
        case ColorName::RED:    return pixels.Color(255, 0, 0);
        case ColorName::BLUE:   return pixels.Color(0, 0, 255);
        case ColorName::ORANGE: return pixels.Color(255, 80, 0);
        case ColorName::OFF:    return pixels.Color(0, 0, 0);
        default:                return pixels.Color(0, 0, 0);
    }
}

void LedController::setColor(int index, ColorName colorName) {
    uint32_t color = getColorValue(colorName);
    if (index >= 0 && index < NumberOfLEDs) {
        pixels.setPixelColor(index, color);
        pixels.show();
    }
}

void LedController::setGlobalColor(ColorName colorName) {
    uint32_t color = getColorValue(colorName);
    pixels.fill(color, 0, NumberOfLEDs);
    pixels.show();
}

void LedController::begin() {
    pixels.begin();
    pixels.clear();
    pixels.show();
}

void LedController::setColor(int index, uint32_t color) {
    if (index >= 0 && index < NumberOfLEDs) {
        pixels.setPixelColor(index, color);
            pixels.show();

    }
}

void LedController::setBrightness(uint8_t brightness) {
    pixels.setBrightness(brightness);
}

void LedController::show() {
    pixels.show();
}
