#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include "sound.h"
#include "tile_engine.h"
#include <cmath>
#include <memory>

Tower::Tower(const sf::Vector2f& worldPos, const TowerStats& stats)
    : stats(stats)
{
    // Snap to grid
    gridPosition.x = int(worldPos.x / LevelSystem::get_tile_size());
    gridPosition.y = int(worldPos.y / LevelSystem::get_tile_size());
    position = LevelSystem::get_screen_coord_at_grid_coord(gridPosition);

    baseShape.setRadius(20.f);
    baseShape.setOrigin(20.f, 20.f);
    baseShape.setFillColor(sf::Color::Blue);
    baseShape.setPosition(position);
}

void Tower::update(float dt,
    const std::vector<std::shared_ptr<Enemy>>& enemies,
    std::vector<std::shared_ptr<Projectile>>& projectiles)
{
    fireCooldown -= dt;

    auto target = findTarget(enemies);
    if (!target) return;

    sf::Vector2f aim = target->getPosition() - position;
    float len = std::sqrt(aim.x * aim.x + aim.y * aim.y);
    if (len == 0.f) return; // safeguard
    aim /= len;

    if (fireCooldown <= 0.f) {
        projectiles.push_back(
            std::make_shared<Projectile>(
                position,
                aim * stats.projectileSpeed,
                stats.projectileDamage
                )
        );
        SoundManager::playSound("tower_fire", 70.f);
        fireCooldown = 1.f / stats.fireRate;
    }
}

std::shared_ptr<Enemy> Tower::findTarget(const std::vector<std::shared_ptr<Enemy>>& enemies) {
    std::shared_ptr<Enemy> best = nullptr;
    float closest = stats.range * LevelSystem::get_tile_size();

    for (auto& e : enemies) {
        sf::Vector2f delta = e->getPosition() - position;
        float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (dist < closest) {
            closest = dist;
            best = e;
        }
    }
    return best;
}

void Tower::render(sf::RenderWindow& window) {
    window.draw(baseShape);
}