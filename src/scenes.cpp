#include "scenes.h"
#include "tower.h"
#include "sound.h"
#include "enemy.h"
#include "button.h"
#include "projectile.h"
#include "engine/renderer.h"
#include "engine/tile_engine.h"
#include "engine/game_system.h"
#include "game_parameters.h"
#include "game_entities.h"
#include <iostream>
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

void MenuScene::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter)
    {
        GameSystem::setActiveScene(Scenes::game);
    }
}

void MenuScene::update(sf::Time delta) {}
void MenuScene::draw(sf::RenderWindow& window) { window.draw(title); }

// ============================================================================
// GAME SCENE
// ============================================================================
// -------------------------
// GameScene constructor
// -------------------------
GameScene::GameScene() {
    // Only default initializations here
    spawnTimer = sf::Time::Zero;
    spawnDelay = sf::seconds(2.f);
    towerMenuOpen = false;
    selectedTower = nullptr;
    selectedTowerCost = 0;
}

void GameScene::load() {
    // Load level
    try {
        LevelSystem::load_level("res/levels/temp_level.txt", 100.f);
    }
    catch (const std::string& err) {
        std::cerr << "Failed to load level: " << err << std::endl;
    }

    // Background music
    SoundManager::initBackgroundMusic("res/sound/__rhodesmas__music-loop.wav");

    // Load font
    if (!moneyFont.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
        std::cerr << "Failed to load font for money display.\n";
    }

    // Money text
    moneyText.setFont(moneyFont);
    moneyText.setCharacterSize(24);
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(10.f, 10.f);

    // Player
    player.setRadius(25.f);
    player.setFillColor(sf::Color::Red);
    player.setOrigin(player.getRadius(), player.getRadius());
    player.setPosition(200.f, 200.f);

    // Preview shape
    previewShape.setFillColor(sf::Color(255, 0, 0, 120));

    // MENU BUTTON
    menuButton = std::make_shared<Button>(
        sf::Vector2f(10.f, 50.f),
        sf::Vector2f(120.f, 40.f),
        "Menu",
        moneyFont,
        [this]() { towerMenuOpen = !towerMenuOpen; }
    );

    // Initialize tower buttons
    initTowerButtons();
}

// -------------------------
// Initialize tower buttons
// -------------------------
void GameScene::initTowerButtons() {
    towerButtons.clear();

    struct TowerButtonInfo {
        std::string name;
        const TowerStats* stats;
        int cost;

        TowerButtonInfo(const std::string& n, const TowerStats* s, int c)
            : name(n), stats(s), cost(c) {}
    };

    std::vector<TowerButtonInfo> towerInfos{
        {"Basic Tower", &BASIC_TOWER, 50},
        {"Cannon Tower", &CANNON_TOWER, 100},
        {"Sniper Tower", &SNIPER_TOWER, 150}
    };

    const float buttonWidth = 120.f;
    const float buttonHeight = 40.f;
    const float padding = 10.f;
    const int perRow = 3;

    for (size_t i = 0; i < towerInfos.size(); ++i) {
        int row = int(i / perRow);
        int col = int(i % perRow);

        sf::Vector2f pos(
            10.f + col * (buttonWidth + padding),
            100.f + row * (buttonHeight + padding)
        );

        const TowerStats* statsPtr = towerInfos[i].stats;
        int cost = towerInfos[i].cost;
        std::string label = towerInfos[i].name;

        towerButtons.push_back(std::make_shared<Button>(
            pos,
            sf::Vector2f(buttonWidth, buttonHeight),
            label,
            moneyFont,
            [this, statsPtr, cost]() {
                selectedTower = statsPtr;
        selectedTowerCost = cost;
        towerMenuOpen = false;
            }
        ));
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

void GameScene::spawnEnemy() {
    EnemyStats stats;

    int r = rand() % 3;
    switch (r) {
    case 0: stats = FAST_ENEMY; break;
    case 1: stats = NORMAL_ENEMY; break;
    case 2: stats = TANK_ENEMY; break;
    }

    auto path = LevelSystem::get_path();
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

// ============================================================================
// EVENT HANDLING
// ============================================================================
void GameScene::handleEvent(const sf::Event& event) {
    sf::RenderWindow& window = Renderer::get_window();

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(
            { event.mouseButton.x, event.mouseButton.y });

        // Tower menu click
        towerMenu.handleClick(mousePos);
        selectedTower = towerMenu.getSelectedTower();
        selectedTowerCost = towerMenu.getSelectedTowerCost();

        // Tower placement
        if (selectedTower && money >= selectedTowerCost && can_place_turret(mousePos)) {
            sf::Vector2i gridPos(
                int(mousePos.x / LevelSystem::get_tile_size()),
                int(mousePos.y / LevelSystem::get_tile_size())
            );
            sf::Vector2f placePos = LevelSystem::get_screen_coord_at_grid_coord(gridPos);

            towers.push_back(std::make_shared<Tower>(placePos, *selectedTower));
            money -= selectedTowerCost;
            selectedTower = nullptr; // single placement
        }
    }
}

// ============================================================================
// UPDATE + DRAW
// ============================================================================
void GameScene::update(sf::Time delta) {
    float dt = delta.asSeconds();
    spawnTimer += delta;

    if (spawnTimer >= spawnDelay) {
        spawnTimer = sf::Time::Zero;
        spawnEnemy();
    }

    for (auto& e : enemies)     e->update(dt);
    for (auto& t : towers)      t->update(dt, enemies, projectiles);
    for (auto& p : projectiles) p->update(dt);

    handleCollisions();

    // Mouse preview
    sf::RenderWindow& window = Renderer::get_window();
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sf::Vector2i mouseGrid(
        int(mousePos.x / LevelSystem::get_tile_size()),
        int(mousePos.y / LevelSystem::get_tile_size())
    );

    sf::Vector2f previewPos(-1000.f, -1000.f);
    bool gridValid = !(mouseGrid.x < 0 || mouseGrid.y < 0 ||
                       mouseGrid.x >= LevelSystem::get_width() || mouseGrid.y >= LevelSystem::get_height());

    if (gridValid)
        previewPos = LevelSystem::get_screen_coord_at_grid_coord(mouseGrid);

    previewShape.setRadius(LevelSystem::get_tile_size() * 0.5f);
    previewShape.setOrigin(previewShape.getRadius(), previewShape.getRadius());
    previewShape.setPosition(previewPos);

    bool allowed = selectedTower && gridValid &&
                   LevelSystem::get_tile_at_grid_coord(mouseGrid) == LevelSystem::EMPTY &&
                   money >= selectedTowerCost;

    previewShape.setFillColor(
        allowed ? sf::Color(0, 255, 0, 120) : sf::Color(255, 0, 0, 120)
    );

    moneyText.setString("Money: " + std::to_string(money));
}

void GameScene::draw(sf::RenderWindow& window) {
    LevelSystem::render();
    Renderer::render();

    for (auto& t : towers)      t->render(window);
    for (auto& e : enemies)     e->render(window);
    for (auto& p : projectiles) p->render(window);

    window.draw(previewShape);
    window.draw(player);
    window.draw(moneyText);

    towerMenu.render(window); // always render menu
}

void GameScene::unload() {
    towers.clear();
    enemies.clear();
    projectiles.clear();
}

