#include "LED.h"

void edit_led_state(const std::array<int, 3> &pins, const std::array<int, 3> &rgb)
{
    for (int i = 0; i < 3; i++)
    {
        // On s'assure que les pins sont bien en mode OUTPUT
        // (Peut être fait une seule fois dans le setup, mais par sécurité :)
        pinMode(pins[i], OUTPUT); 
        analogWrite(pins[i], rgb[i]);
    }
}