#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "engine/scene.h"

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> game;
};

// Forward declarations
class Tower;
class Enemy;
class Projectile;

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
    int lives = 20;
    int money = 100;               // Player currency
    sf::Font font;                 // Font for displaying text
    sf::Text moneyText;            // Text object

    void spawnEnemy();
    void handleCollisions();
    bool can_place_turret(sf::Vector2f mousePos);
};