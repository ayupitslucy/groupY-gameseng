#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct EnemyStats {
    int health;
    float speed;
    float armour;
    float rewardMultiplier = 1.f;
};

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, const EnemyStats& stats);

    void update(float dt);
    void render(sf::RenderWindow& window);
    void takeDamage(int dmg);

    bool isDead() const { return health <= 0; }
    sf::Vector2f getPosition() const;

    float getRewardMultiplier() const { return rewardMultiplier; }

    // Call once at startup
    static bool loadTextures();

private:
    std::vector<sf::Vector2f> path;
    size_t pathIndex;
    float speed;
    float armour;
    int health;
    float rewardMultiplier;

    // Visuals
    sf::Sprite sprite;
    sf::CircleShape fallbackShape;

    static sf::Texture fastTex;
    static sf::Texture normalTex;
    static sf::Texture tankTex;
    static bool texturesLoaded;
    bool useSprite = false;
};