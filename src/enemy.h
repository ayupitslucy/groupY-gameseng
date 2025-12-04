#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct EnemyStats {
    int health;
    float speed;
    float armour; // 0.2 = 20% damage reduction
    float rewardMultiplier = 1.f; // NEW
};

class Enemy {
public:
    // Change path type to Vector2f (screen coordinates)
    Enemy(const std::vector<sf::Vector2f>& path, const EnemyStats& stats);

    void update(float dt);
    void render(sf::RenderWindow& window);
    void takeDamage(int dmg);

    bool isDead() const { return health <= 0; }
    sf::Vector2f getPosition() const;

    float getRewardMultiplier() const { return rewardMultiplier; }

private:
    std::vector<sf::Vector2f> path;  // screen coordinates
    size_t pathIndex;
    float speed;
    float armour;
    int health;
    float rewardMultiplier;

    sf::CircleShape shape;
};