#pragma once
#include <SFML/Graphics.hpp>
#include "entity_manager.h"

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void load() = 0;
    virtual void unload() {}

    // Event handling
    virtual void handleEvent(const sf::Event& event) {}

    // Frame update
    virtual void update(sf::Time dt) {}

    // Frame rendering
    virtual void draw(sf::RenderWindow& window) {}
};