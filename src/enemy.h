#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed);

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;

private:
    sf::CircleShape shape;
    const std::vector<sf::Vector2f>* path;
    std::size_t pathIndex;
    float speed;
    sf::Vector2f velocity;
};