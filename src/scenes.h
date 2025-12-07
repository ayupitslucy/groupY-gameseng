#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "engine/scene.h"
#include "tower.h"
#include "button.h"
#include "TowerMenu.h"

// Forward declarations
class Enemy;
class Projectile;

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> game;
};

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
    sf::Text enter;
};

// ============================================================================
// GAME SCENE
// ============================================================================
class GameScene : public Scene {
public:
    GameScene();
    void load() override;
    void unload() override;
    void handleEvent(const sf::Event& event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderWindow& window) override;

private:
    std::vector<std::shared_ptr<Tower>> towers;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Projectile>> projectiles;

    TowerMenu towerMenu;
    std::vector<std::shared_ptr<Button>> towerButtons;
    bool towerMenuOpen = false;
    const TowerStats* selectedTower = nullptr;
    int selectedTowerCost = 0;

    sf::CircleShape previewShape;
    sf::CircleShape player;

    sf::Font moneyFont;
    sf::Text moneyText;

    sf::Time spawnTimer;
    sf::Time spawnDelay;

    int lives = 20;
    int money = 100;

    void spawnEnemy();
    void handleCollisions();
    bool can_place_turret(sf::Vector2f mousePos);

    std::shared_ptr<Button> menuButton;

    void initTowerButtons();

};