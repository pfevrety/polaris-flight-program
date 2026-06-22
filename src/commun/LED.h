#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <array>

namespace LedConfig {
    inline constexpr std::array<int, 3> PIN_LED_DEBUG_LORA = {1, 2, 3};

    inline constexpr std::array<int, 3> RGB_OK = {0, 255, 0};        // Vert
    inline constexpr std::array<int, 3> RGB_EN_COURS = {0, 0, 255};  // Bleu
    inline constexpr std::array<int, 3> RGB_PAS_OK = {255, 0, 0};    // Rouge
    inline constexpr std::array<int, 3> RGB_ATTENTE = {255, 128, 0}; // Orange
}

// Prototype de la fonction
void edit_led_state(const std::array<int, 3> &pins, const std::array<int, 3> &rgb);

#endif