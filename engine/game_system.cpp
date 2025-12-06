#include "game_system.h"

std::shared_ptr<Scene> GameSystem::active_scene = nullptr;
bool GameSystem::quitRequested = false;

void GameSystem::setActiveScene(std::shared_ptr<Scene> scene) {
    if (active_scene) {
        active_scene->unload();
    }
    active_scene = scene;
    if (active_scene) {
        active_scene->load();
    }
}

void GameSystem::handleEvent(const sf::Event& event) {
    if (active_scene) {
        active_scene->handleEvent(event);
    }
}

void GameSystem::update(sf::Time dt) {
    if (active_scene) {
        active_scene->update(dt);
    }
}

void GameSystem::draw(sf::RenderWindow& window) {
    if (active_scene) {
        active_scene->draw(window);
    }
}

void GameSystem::requestQuit() {
    quitRequested = true;
}

bool GameSystem::shouldQuit() {
    return quitRequested;
}