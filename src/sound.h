#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class SoundManager {
public:
    // Background music
    static bool initBackgroundMusic(const std::string& filename, float volume = 50.f);
    static void stopBackgroundMusic();
    static void setBackgroundVolume(float volume);
    static void pauseBackgroundMusic();
    static void resumeBackgroundMusic();

    // Sound effects
    static void loadSound(const std::string& name, const std::string& filename);
    static void playSound(const std::string& name, float volume = 100.f);
    static void update(); // clean up finished sounds

private:
    // Background music
    static sf::Music backgroundMusic;

    // SFX
    static std::unordered_map<std::string, sf::SoundBuffer> buffers;
    static std::vector<sf::Sound> activeSounds;
};