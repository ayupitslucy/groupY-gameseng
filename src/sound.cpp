#include "sound.h"
#include <SFML/Audio.hpp>
#include <iostream>

static sf::Music g_backgroundMusic;

bool initBackgroundMusic()
{
    if (!g_backgroundMusic.openFromFile("res/sound/__rhodesmas__music-loop.wav"))
    {
        std::cerr << "Error: could not load background music\n";
        return false;
    }

   // g_backgroundMusic.setLoop(true);
   // g_backgroundMusic.setVolume(50.f);
    g_backgroundMusic.play();

    return true;
}

void stopBackgroundMusic()
{
    g_backgroundMusic.stop();
}
