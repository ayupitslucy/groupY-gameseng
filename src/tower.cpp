#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include <cmath>
#include <algorithm>

Tower::Tower(sf::Vector2f position) : pos(position) {}

void Tower::update(
    float dt,
    const std::vector<std::shared_ptr<Enemy>>& enemies,
    std::vector<std::shared_ptr<Projectile>>& projectiles
) {
    fireCooldown -= dt;

    auto target = aimAtEnemy(enemies);
    if (target) {
        sf::Vector2f aimDir = computeAimDirection(*target);

        if (fireCooldown <= 0.f) {
            projectiles.push_back(
                std::make_shared<Projectile>(pos, aimDir * projectileSpeed)
            );
            fireCooldown = 1.f / fireRate;
        }
    }
}

void Tower::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

std::shared_ptr<Enemy> Tower::aimAtEnemy(
    const std::vector<std::shared_ptr<Enemy>>& enemies
) {
    std::shared_ptr<Enemy> target = nullptr;
    float closestDist = range;

    for (auto& e : enemies) {
        if (!e) continue;

        sf::Vector2f diff = e->getPosition() - pos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= closestDist) {
            closestDist = dist;
            target = e;
        }
    }
    return target;
}

sf::Vector2f Tower::computeAimDirection(const Enemy& enemy) {
    sf::Vector2f shooterPos = pos;
    sf::Vector2f targetPos = enemy.getPosition();
    sf::Vector2f targetVel = enemy.getVelocity();

    sf::Vector2f dir = targetPos - shooterPos;
    float a = targetVel.x * targetVel.x + targetVel.y * targetVel.y
        - projectileSpeed * projectileSpeed;
    float b = 2.f * (dir.x * targetVel.x + dir.y * targetVel.y);
    float c = dir.x * dir.x + dir.y * dir.y;

    float disc = b * b - 4 * a * c;
    if (disc < 0.f) {
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        return dir / len;
    }

    float t1 = (-b + std::sqrt(disc)) / (2.f * a);
    float t2 = (-b - std::sqrt(disc)) / (2.f * a);
    float t = (t1 > 0 ? t1 : t2);
    if (t < 0) t = std::max(t1, t2);

    sf::Vector2f aimPoint = targetPos + targetVel * t;
    sf::Vector2f aimDir = aimPoint - shooterPos;
    float len = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
    return aimDir / len;
}