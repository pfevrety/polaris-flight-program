#ifndef SPEAKER_H
#define SPEAKER_H

#include <DFMiniMp3.h>
#include <array>
#include <cstring>
#include <Arduino.h>

struct Pair
{
    const char* key;   // Nom de l'événement
    const int value;   // Index du fichier MP3 associé
};

// Dictionnaire constant associant chaque événement à un fichier audio
constexpr std::array<Pair, 1> conversionMap = {{
    {"STARTUP", 1} // Exemple de son au démarrage
}};

// Typedef pour simplifier le DFPlayer Mini
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 

// Recherche l'index MP3 correspondant à une clé
inline int lookup(const char* key)
{
    for (const auto& p : conversionMap)
    {
        if (strcmp(p.key, key) == 0)
            return p.value;
    }
    return -1; // Retourne -1 si la clé n'est pas trouvée
}

// Classe pour gérer le DFPlayer Mini et jouer des sons
class Speaker
{
private:
    DfMp3 dfmp3;

public:
    // Constructeur
    Speaker(HardwareSerial &serial) : dfmp3(serial) {}

    // Initialise le DFPlayer et règle le volume
    void begin(int volume = 20);

    // Joue le son correspondant à une clé d'événement
    void play(const char* key);
};

#endif // SPEAKER_H
