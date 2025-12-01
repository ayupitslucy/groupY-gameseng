#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f pos, sf::Vector2f vel);

    sf::Vector2f getPosition() const { return shape.getPosition(); } // only this

    void update(float dt);
    void render(sf::RenderWindow& window);

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
};