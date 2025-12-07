#include "projectile.h"
#include <SFML/Graphics/CircleShape.hpp>  // REQUIRED in SFML 2.6.2

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f vel, int damage)
    : velocity(vel), damage(damage)
{
    shape.setRadius(5.f);
    shape.setOrigin(5.f, 5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(pos);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(shape);
}