#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "scene.h"

class GameSystem {
public:
    static void setActiveScene(std::shared_ptr<Scene> scene);

    static void handleEvent(const sf::Event& event);
    static void update(sf::Time dt);
    static void draw(sf::RenderWindow& window);

    static void requestQuit();
    static bool shouldQuit();

private:
    static std::shared_ptr<Scene> active_scene;
    static bool quitRequested;
};