#include "projectile.h"

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f vel)
    : velocity(vel)
{
    shape.setRadius(5.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(pos);
}

void Projectile::update(float dt) {
    shape.move(velocity * dt);
}

void Projectile::render(sf::RenderWindow& window) {
    window.draw(shape);
}