// sound.h
#pragma once
#include <SFML/Audio.hpp>

class SoundManager {
public:
    static bool initBackgroundMusic(const std::string& filename, float volume = 50.f);
    static void stopBackgroundMusic();
    static void setBackgroundVolume(float volume);
    static void pauseBackgroundMusic();
    static void resumeBackgroundMusic();

private:
    static sf::Music backgroundMusic;
};