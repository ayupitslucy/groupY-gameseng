#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Enemy;
class Projectile;

class Tower {
public:
    Tower(sf::Vector2f position);

    void update(
        float dt,
        const std::vector<std::shared_ptr<Enemy>>& enemies,
        std::vector<std::shared_ptr<Projectile>>& projectiles
    );

    void render(sf::RenderWindow& window);

private:
    sf::Vector2f pos;
    sf::Sprite sprite;

    float fireCooldown = 0.f;
    float fireRate = 1.f;
    float projectileSpeed = 300.f;
    float range = 200.f;

    std::shared_ptr<Enemy> aimAtEnemy(
        const std::vector<std::shared_ptr<Enemy>>& enemies
    );

    sf::Vector2f computeAimDirection(const Enemy& enemy);
};