#include "enemy.h"
#include "tile_engine.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::Vector2f>& path, const EnemyStats& stats)
    : path(path), pathIndex(0), speed(stats.speed),
    armour(stats.armour), health(stats.health)
{
    shape.setRadius(20.f);
    shape.setOrigin(20.f, 20.f);
    shape.setFillColor(sf::Color::Green);

    if (!path.empty())
        shape.setPosition(path[0]);
}

void Enemy::update(float dt) {
    if (pathIndex >= path.size()) return;

    sf::Vector2f target = path[pathIndex];
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f dir = target - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < speed * dt) {
        pathIndex++;
    }
    else {
        sf::Vector2f norm = dir / len;
        shape.move(norm * speed * dt);
    }
}

void Enemy::takeDamage(int dmg) {
    int effective = dmg - int(dmg * armour);
    if (effective < 1) effective = 1;
    health -= effective;
    shape.setFillColor(sf::Color(255, 200, 0));
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const {
    return shape.getPosition();
}