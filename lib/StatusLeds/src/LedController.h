#pragma once
#include <Adafruit_NeoPixel.h>

class LedController {
public:
    enum class Color : uint8_t {
        OFF    = 0,
        GREEN  = 1,
        RED    = 2,
        BLUE   = 3,
        ORANGE = 4,
        WHITE  = 5,
        PURPLE = 6,
        YELLOW = 7
    };

    struct BlinkConfig {
        bool     active       = false;
        Color    colorOn      = Color::OFF;
        Color    colorOff     = Color::OFF;
        uint32_t intervalMs   = 500;
        uint32_t lastToggleMs = 0;
        bool     state        = false;
    };

    LedController(int ledCount, int ledPin);

    void begin();
    void setBrightness(uint8_t brightness);
    void setGlobalBlink(Color colorOn, uint32_t intervalMs, Color colorOff = Color::OFF);
    void stopGlobalBlink(Color finalColor);
    void setColor(int index, Color color);
    void setGlobalColor(Color color);

    void setBlink(int index, Color colorOn, uint32_t intervalMs, Color colorOff = Color::OFF);
    void stopBlink(int index, Color finalColor = Color::OFF);

    void update();

    void show();

private:
    Adafruit_NeoPixel pixels;
    int               ledCount;
    BlinkConfig*      blinkConfigs;

    uint32_t getColorValue(Color color) const;
};
