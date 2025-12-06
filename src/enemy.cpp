#include "enemy.h"
#include "game_entities.h"   // provides FAST_ENEMY etc.
#include <iostream>
#include <cmath>

// Static texture defs
sf::Texture Enemy::fastTex;
sf::Texture Enemy::normalTex;
sf::Texture Enemy::tankTex;
bool Enemy::texturesLoaded = false;

// Load textures once. Returns true if at least one texture loaded successfully.
bool Enemy::loadTextures()
{
    bool anyLoaded = false;
    texturesLoaded = true; // mark attempted

    if (fastTex.loadFromFile("res/icons_png/toast.png")) {
        std::cout << "[OK] fast texture loaded\n";
        anyLoaded = true;
    }
    else {
        std::cerr << "[WARN] fast texture failed to load (res/icons_png/toast.png)\n";
    }

    if (normalTex.loadFromFile("res/icons_png/egg.png")) {
        std::cout << "[OK] normal texture loaded\n";        // debugging stuff
        anyLoaded = true;
    }
    else {
        std::cerr << "[WARN] normal texture failed to load (res/icons_png/egg.png)\n";
    }

    if (tankTex.loadFromFile("res/icons_png/bacon.png")) {
        std::cout << "[OK] tank texture loaded\n";
        anyLoaded = true;
    }
    else {
        std::cerr << "[WARN] tank texture failed to load (res/icons_png/bacon.png)\n";
    }

    return anyLoaded;
}

Enemy::Enemy(const std::vector<sf::Vector2f>& path, const EnemyStats& stats)
    : path(path),
    pathIndex(0),
    speed(stats.speed),
    armour(stats.armour),
    health(stats.health),
    rewardMultiplier(stats.rewardMultiplier),
    useSprite(false)
{
    // Ensure textures were attempted to load (call loadTextures() at startup)
    // but be defensive: if not called, attempt now (non-fatal).
    if (!texturesLoaded) {
        loadTextures();
    }

    // Decide which static texture to use, but only set sprite if texture has data.
    const sf::Texture* chosen = nullptr;

    if (&stats == &FAST_ENEMY) {
        if (fastTex.getSize().x > 0) chosen = &fastTex;
    }
    else if (&stats == &TANK_ENEMY) {
        if (tankTex.getSize().x > 0) chosen = &tankTex;
    }
    else /* normal or default */ {
        if (normalTex.getSize().x > 0) chosen = &normalTex;
    }

    if (chosen != nullptr) {
        sprite.setTexture(*chosen);
        // safe: chosen is valid, so getSize is OK
        sprite.setOrigin(chosen->getSize().x / 2.f, chosen->getSize().y / 2.f);
        useSprite = true;
    }
    else {
        useSprite = false;
        // fallback shape
        fallbackShape.setRadius(20.f);
        fallbackShape.setOrigin(20.f, 20.f);
        fallbackShape.setFillColor(sf::Color::Green);
    }

    // Set initial position (both sprite and fallback)
    if (!path.empty()) {
        if (useSprite) sprite.setPosition(path[0]);
        fallbackShape.setPosition(path[0]);
    }
}

void Enemy::update(float dt)
{
    if (pathIndex >= path.size()) return;

    sf::Vector2f pos = useSprite ? sprite.getPosition() : fallbackShape.getPosition();
    sf::Vector2f target = path[pathIndex];
    sf::Vector2f dir = target - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (len < speed * dt) {
        pathIndex++;
    }
    else {
        sf::Vector2f norm = dir / len;
        sf::Vector2f move = norm * speed * dt;
        if (useSprite) sprite.move(move);
        else fallbackShape.move(move);
    }
}

bool Enemy::takeDamage(int dmg) {
    int effective = dmg - int(dmg * armour);
    if (effective < 1) effective = 1;
    health -= effective;
    
        if (!useSprite) fallbackShape.setFillColor(sf::Color(255, 200, 0));
    else sprite.setColor(sf::Color(255, 200, 200));
            // return true if now dead
        return (health <= 0);
}

void Enemy::render(sf::RenderWindow& window)
{
    if (useSprite && sprite.getTexture() != nullptr)
        window.draw(sprite);
    else
        window.draw(fallbackShape);
}

sf::Vector2f Enemy::getPosition() const
{
    return useSprite ? sprite.getPosition() : fallbackShape.getPosition();
}