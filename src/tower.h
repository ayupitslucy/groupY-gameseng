#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct TowerStats {
    float range;        // in tiles
    float fireRate;     // shots per second
    int projectileDamage;
    float projectileSpeed;
};

class Enemy; // forward declare
class Projectile;

class Tower {
public:
    // Accept either grid or world position
    Tower(const sf::Vector2f& worldPos, const TowerStats& stats);

    void update(float dt,
        const std::vector<std::shared_ptr<Enemy>>& enemies,
        std::vector<std::shared_ptr<Projectile>>& projectiles
    );

    void render(sf::RenderWindow& window);

private:
    sf::Vector2i gridPosition; // snapped to LevelSystem grid
    sf::Vector2f position;     // actual screen position
    TowerStats stats;
    float fireCooldown = 0.f;

    sf::CircleShape baseShape;

    std::shared_ptr<Enemy> findTarget(const std::vector<std::shared_ptr<Enemy>>& enemies);
};