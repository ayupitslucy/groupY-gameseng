#pragma once
#include <memory>
#include "engine/scene.h"

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> game;
};

class Button {
public:
    sf::RectangleShape shape;
    bool isHovered = false;

    void update(sf::Vector2f mousePos) {
        if (shape.getGlobalBounds().contains(mousePos)) {
            isHovered = true;
            shape.setFillColor(sf::Color::Yellow); // hover color
        }
        else {
            isHovered = false;
            shape.setFillColor(sf::Color::White);
        }
    }
};

class MenuScene : public Scene {
public:
    MenuScene() = default;
    void update(const float& delta_time) override;
    void render() override;
    void load() override;
private:
    std::unique_ptr<sf::Text> text;
    std::unique_ptr<sf::Font> font;
};

class GameScene : public Scene {
public:
    GameScene() = default;
    void update(const float& delta_time) override;
    void render() override;
    void load() override;

private:
    std::shared_ptr<Entity> player;

    // might be better to declare this in a seperate file
    struct Tower {
        sf::CircleShape shape;
        sf::Vector2f pos;
    };

    struct Enemy {
        sf::CircleShape shape;
        std::vector<sf::Vector2f>* path;
        int pathIndex = 0;
        float speed = 75.f;
    };

    std::vector<Tower> towers;
    std::vector<Enemy> enemies;

    void spawn_enemy();
    void update_enemies(float dt);

    bool can_place_turret(sf::Vector2f mousePos);
    void place_turret(sf::Vector2f mousePos);

    float spawnTimer = 0.f;
    float spawnDelay = 2.f;
};
