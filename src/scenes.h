#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "engine/scene.h"

// Forward declarations
class Tower;
class Enemy;
class Projectile;

//// ============================================================================
//// BASE SCENE CLASS
//// ============================================================================
//class Scene {
//public:
//    virtual ~Scene() = default;
//
//    virtual void handleEvent(sf::Event& event) = 0;
//    virtual void update(sf::Time delta) = 0;
//    virtual void draw(sf::RenderWindow& window) = 0;
//};

// ============================================================================
// MENU SCENE
// ============================================================================
class MenuScene : public Scene {
public:
    MenuScene();

    void handleEvent(const sf::Event& event);
    void update(sf::Time delta);
    void draw(sf::RenderWindow& window);
    virtual void load() {}

private:
    sf::Font font;
    sf::Text title;
};

// ============================================================================
// GAME SCENE
// ============================================================================
class GameScene : public Scene {
public:
    GameScene();

    void handleEvent(const sf::Event& event); // FIXED
    void update(sf::Time delta);
    void draw(sf::RenderWindow& window);
    virtual void load() {}

private:
    // Gameplay objects
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    // Player example
    sf::CircleShape player;

    // Tower placement preview
    sf::CircleShape previewShape;

    // Enemy spawn timing
    sf::Time spawnTimer;
    sf::Time spawnDelay;

private:
    void spawnEnemy();
    void handleCollisions();
    bool can_place_turret(sf::Vector2f mousePos);
};