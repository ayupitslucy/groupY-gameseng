#include "sound.h"
#include <SFML/Audio.hpp>
#include <iostream>

sf::Music SoundManager::backgroundMusic;

bool SoundManager::initBackgroundMusic(const std::string& filename, float volume)
{
    if (!backgroundMusic.openFromFile(filename))
    {
        std::cerr << "Error: could not load background music: " << filename << "\n";
        return false;
    }

    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(volume);
    backgroundMusic.play();

    return true;
}

void SoundManager::stopBackgroundMusic()
{
    backgroundMusic.stop();
}

void SoundManager::setBackgroundVolume(float volume)
{
    backgroundMusic.setVolume(volume);
}

void SoundManager::pauseBackgroundMusic()
{
    backgroundMusic.pause();
}

void SoundManager::resumeBackgroundMusic()
{
    backgroundMusic.play();
}
