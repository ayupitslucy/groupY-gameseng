#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f pos, sf::Vector2f vel, int damage);

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const { return shape.getPosition(); }
    int getDamage() const { return damage; }

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int damage;
};