#include "sound.h"
#include <iostream>
#include <algorithm>

// Background music
sf::Music SoundManager::backgroundMusic;

// Sound effect storage
std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::vector<sf::Sound> SoundManager::activeSounds;

bool SoundManager::initBackgroundMusic(const std::string& filename, float volume) {
    if (!backgroundMusic.openFromFile(filename)) {
        std::cerr << "Error: could not load background music: " << filename << "\n";
        return false;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(volume);
    backgroundMusic.play();
    return true;
}

void SoundManager::stopBackgroundMusic() { backgroundMusic.stop(); }
void SoundManager::setBackgroundVolume(float volume) { backgroundMusic.setVolume(volume); }
void SoundManager::pauseBackgroundMusic() { backgroundMusic.pause(); }
void SoundManager::resumeBackgroundMusic() { backgroundMusic.play(); }

// ------------------ SFX ------------------
void SoundManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "[SFX] Failed to load: " << filename << "\n";
        return;
    }
    buffers[name] = buffer;
}

void SoundManager::playSound(const std::string& name, float volume) {
    auto it = buffers.find(name);
    if (it == buffers.end()) return;

    sf::Sound sound;
    sound.setBuffer(it->second);
    sound.setVolume(volume);
    sound.play();

    activeSounds.push_back(sound);
}

void SoundManager::update() {
    // remove finished sounds
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
        activeSounds.end()
    );
}