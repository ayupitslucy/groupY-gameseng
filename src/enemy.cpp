#include "enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& path, float speed)
    : path(&path), pathIndex(0), speed(speed), velocity(0.f, 0.f)
{
    shape = sf::CircleShape(20.f);
    shape.setOrigin(20.f, 20.f);
    shape.setFillColor(sf::Color::Green);

    if (!path.empty()) shape.setPosition(path[0]);
}

void Enemy::update(float dt)
{
    if (!path || pathIndex >= path->size()) {
        velocity = sf::Vector2f(0.f, 0.f);
        return;
    }

    sf::Vector2f target = (*path)[pathIndex];
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f dir = target - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < 2.f) {
        pathIndex++;
        velocity = sf::Vector2f(0.f, 0.f);
        return;
    }

    sf::Vector2f norm = dir / len;
    velocity = norm * speed;
    shape.move(velocity * dt);
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const { return shape.getPosition(); }
sf::Vector2f Enemy::getVelocity() const { return velocity; }