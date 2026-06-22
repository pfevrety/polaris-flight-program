#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Adafruit_NeoPixel.h>

class LedController {
public:
    enum ColorName {
        GREEN,
        RED,
        BLUE,
        ORANGE,
        OFF
    };

    LedController(int LED_COUNT, int LED_PIN);
    void begin();
    
    void setColor(int index, ColorName colorName);
    void setColor(int index, uint32_t color);
    void setGlobalColor(ColorName colorName);
    void setBrightness(uint8_t brightness);
    void show();

private:
    int NumberOfLEDs;
    Adafruit_NeoPixel pixels;
    uint32_t getColorValue(ColorName colorName);
};

#endif