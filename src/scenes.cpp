#include "scenes.h"
#include "tower.h"
#include "enemy.h"
#include "projectile.h"
#include "engine/renderer.h"
#include "engine/tile_engine.h"
#include "engine/game_system.h"
#include "game_parameters.h"
#include "game_entities.h"
#include <unordered_set>

#include <iostream>   // for std::cerr

#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::game;

// ============================================================================
// MENU SCENE
// ============================================================================
MenuScene::MenuScene() {
    font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
    title.setFont(font);
    title.setString("Main Menu");
    title.setCharacterSize(50);
    title.setPosition(100.f, 100.f);
}

void MenuScene::handleEvent(const sf::Event& event) {   // FIXED
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter)
    {
        GameSystem::setActiveScene(Scenes::game);
    }
}

void MenuScene::update(sf::Time delta) {
    // (Unused for now—kept clean)
}

void MenuScene::draw(sf::RenderWindow& window) {
    window.draw(title);
}


// ============================================================================
// GAME SCENE
// ============================================================================

GameScene::GameScene() {
    try {
        LevelSystem::load_level("res/levels/temp_level.txt", 100.f);
    }
    catch (const std::string& err) {
        std::cerr << "Failed to load level: " << err << std::endl;
    }
    // adjust path and tile size

    spawnTimer = sf::Time::Zero;
    spawnDelay = sf::seconds(2.f);

    player.setRadius(25.f);
    player.setFillColor(sf::Color::Red);
    player.setOrigin(player.getRadius(), player.getRadius());
    player.setPosition(200.f, 200.f);

    previewShape.setFillColor(sf::Color(255, 0, 0, 120));
}

void GameScene::handleEvent(const sf::Event& event) {   // FIXED
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left)  player.move(-10.f, 0.f);
        if (event.key.code == sf::Keyboard::Right) player.move(10.f, 0.f);
        if (event.key.code == sf::Keyboard::Up)    player.move(0.f, -10.f);
        if (event.key.code == sf::Keyboard::Down)  player.move(0.f, 10.f);
    }

    // Place tower
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = Renderer::get_window().mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });
        if (can_place_turret(mousePos))
            towers.push_back(std::make_shared<Tower>(mousePos, BASIC_TOWER));
    }
}

void GameScene::update(sf::Time delta) {
    float dt = delta.asSeconds();

    // Enemy spawning
    spawnTimer += delta;
    if (spawnTimer >= spawnDelay) {
        spawnTimer = sf::Time::Zero;
        spawnEnemy();
    }

    // Update all objects
    for (auto& e : enemies)     e->update(dt);
    for (auto& t : towers)      t->update(dt, enemies, projectiles);
    for (auto& p : projectiles) p->update(dt);

    handleCollisions();

    // Mouse world position
    sf::RenderWindow& window = Renderer::get_window();
    if (&window == nullptr) return; // safeguard
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    previewShape.setPosition(mousePos);

    // Tower preview
    previewShape.setRadius(LevelSystem::get_tile_size() * 0.5f);
    previewShape.setOrigin(previewShape.getRadius(), previewShape.getRadius());
    previewShape.setPosition(mousePos);

    bool allowed = can_place_turret(mousePos);
    previewShape.setFillColor(
        allowed ? sf::Color(0, 255, 0, 120) : sf::Color(255, 0, 0, 120)
    );
}

void GameScene::draw(sf::RenderWindow& window) {
    LevelSystem::render();
    Renderer::render();   // <--- flush all queued tiles

    for (auto& t : towers)      t->render(window);
    for (auto& e : enemies)     e->render(window);
    for (auto& p : projectiles) p->render(window);

    window.draw(previewShape);
    window.draw(player);
}


// ============================================================================
// PRIVATE HELPERS
// ============================================================================

void GameScene::spawnEnemy() {
    EnemyStats stats;

    int r = rand() % 3;
    switch (r) {
    case 0: stats = FAST_ENEMY; break;
    case 1: stats = NORMAL_ENEMY; break;
    case 2: stats = TANK_ENEMY; break;
    }

    auto path = LevelSystem::get_path(); // returns Vector2f
    if (!path.empty()) {
        enemies.push_back(std::make_shared<Enemy>(path, stats));
    }
}

void GameScene::handleCollisions() {
    for (size_t i = 0; i < projectiles.size(); ) {
        bool hit = false;

        for (size_t j = 0; j < enemies.size(); ++j) {
            sf::Vector2f d = enemies[j]->getPosition() - projectiles[i]->getPosition();
            float dist = std::sqrt(d.x * d.x + d.y * d.y);

            if (dist < 20.f) {
                enemies.erase(enemies.begin() + j);
                hit = true;
                break;
            }
        }

        if (hit) projectiles.erase(projectiles.begin() + i);
        else     ++i;
    }
}

bool GameScene::can_place_turret(sf::Vector2f mousePos) {
    sf::Vector2i grid(
        int(mousePos.x / LevelSystem::get_tile_size()),
        int(mousePos.y / LevelSystem::get_tile_size())
    );

    if (grid.x < 0 || grid.y < 0 ||
        grid.x >= LevelSystem::get_width() ||
        grid.y >= LevelSystem::get_height())
        return false;

    return LevelSystem::get_tile_at_grid_coord(grid) == LevelSystem::EMPTY;
}