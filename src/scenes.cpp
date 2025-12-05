#include "scenes.h"
#include "tower.h"
#include "enemy.h"
#include "button.h"
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

std::shared_ptr<Button> menuButton;
std::vector<std::shared_ptr<Button>> towerButtons;
bool menuOpen = false;

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

    // Load font
    if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font for money display.\n";
    }

    moneyText.setFont(font);
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(10.f, 10.f);

    spawnTimer = sf::Time::Zero;
    spawnDelay = sf::seconds(2.f);

    player.setRadius(25.f);
    player.setFillColor(sf::Color::Red);
    player.setOrigin(player.getRadius(), player.getRadius());
    player.setPosition(200.f, 200.f);

    previewShape.setFillColor(sf::Color(255, 0, 0, 120));

    // MENU BUTTON
    menuButton = std::make_shared<Button>(
        sf::Vector2f(10.f, 50.f), sf::Vector2f(120.f, 40.f),
        "Menu", font,
        [this]() { menuOpen = !menuOpen; }
    );

    // --- CREATE TOWER BUTTONS ONCE ---
    towerButtons.clear();

    towerButtons.push_back(std::make_shared<Button>(
        sf::Vector2f(10.f, 100.f), sf::Vector2f(120.f, 40.f),
        "Basic Tower", font,
        [this]() {
            sf::Vector2f mousePos =
            Renderer::get_window().mapPixelToCoords(
                sf::Mouse::getPosition(Renderer::get_window()));

    if (can_place_turret(mousePos) && money >= 50) {
        towers.push_back(std::make_shared<Tower>(mousePos, BASIC_TOWER));
        money -= 50;
    }
        }
    ));
}

void GameScene::handleEvent(const sf::Event& event) {

    // Player movement
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left)  player.move(-10.f, 0.f);
        if (event.key.code == sf::Keyboard::Right) player.move(10.f, 0.f);
        if (event.key.code == sf::Keyboard::Up)    player.move(0.f, -10.f);
        if (event.key.code == sf::Keyboard::Down)  player.move(0.f, 10.f);
    }

    // Place tower by clicking the world (no button)
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos =
            Renderer::get_window().mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

        if (can_place_turret(mousePos)) {
            towers.push_back(std::make_shared<Tower>(mousePos, BASIC_TOWER));
        }
    }

    // Handle menu button clicks
    menuButton->handleEvent(event, Renderer::get_window());

    // Handle tower button clicks *only when menu is open*
    if (menuOpen) {
        for (auto& btn : towerButtons)
            btn->handleEvent(event, Renderer::get_window());
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

    // Update money display
    moneyText.setString("Money: " + std::to_string(money));
}

void GameScene::draw(sf::RenderWindow& window) {
    LevelSystem::render();
    Renderer::render();   // <--- flush all queued tiles

    for (auto& t : towers)      t->render(window);
    for (auto& e : enemies)     e->render(window);
    for (auto& p : projectiles) p->render(window);

    window.draw(previewShape);
    window.draw(player);

    // Draw money
    window.draw(moneyText);

    menuButton->render(window);
    if (menuOpen) {
        for (auto& btn : towerButtons)
            btn->render(window);
    }
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
    int baseReward = 100;

    for (size_t i = 0; i < projectiles.size(); ) {
        bool hit = false;

        for (size_t j = 0; j < enemies.size(); ++j) {
            sf::Vector2f d = enemies[j]->getPosition() - projectiles[i]->getPosition();
            float dist = std::sqrt(d.x * d.x + d.y * d.y);

            if (dist < 20.f) {
                // Use the enemy's reward multiplier
                money += static_cast<int>(baseReward * enemies[j]->getRewardMultiplier());

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