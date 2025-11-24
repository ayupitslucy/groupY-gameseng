#include "scenes.h"
#include <SFML/Graphics.hpp>
#include "engine/game_system.h"
#include "engine/renderer.h"
#include "engine/tile_engine.h"
#include "game_parameters.h"
#include "components.h"
#include <cmath>

// ================================ Scenes ================================

std::shared_ptr<Scene> Scenes::menu;
std::shared_ptr<Scene> Scenes::game;

// ================================ MenuScene ================================

void MenuScene::update(const float& delta_time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        GameSystem::set_active_scene(Scenes::game);
    }
    Scene::update(delta_time);
}

void MenuScene::render() {
    Renderer::queue(text.get());
    Scene::render();
}

void MenuScene::load() {
    this->font = std::make_unique<sf::Font>();
    this->font->loadFromFile("res/fonts/RobotoMono-Regular.ttf");
    this->text = std::make_unique<sf::Text>(*this->font, "Menu Screen (Press space to play)");
    this->text->setCharacterSize(24);
    // center horizontally
    float x = (Parameters::game_width * .5f) - (this->text->getLocalBounds().width * .5f);
    this->text->setPosition(sf::Vector2f(x, 0));
}

// ================================ GameScene ================================

// helper structs inside scenes.cpp or move to header if you prefer
struct Tower {
    sf::CircleShape shape;
    sf::Vector2f pos;
};

struct Enemy {
    sf::CircleShape shape;
    std::vector<sf::Vector2f>* path = nullptr;
    std::size_t pathIndex = 0;
    float speed = 75.f;
};

void GameScene::load() {
    LevelSystem::load_level(Parameters::level_1, 100.0f);

    this->player = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = this->player->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(50.0f);
    shape->get_shape().setFillColor(sf::Color::Red);
    // SFML's setOrigin takes a Vector2f in some builds?
    shape->get_shape().setOrigin(sf::Vector2f(50.f, 50.f));
    this->player->set_position(LevelSystem::get_start_position() + sf::Vector2f(LevelSystem::get_tile_size() / 2.0f, LevelSystem::get_tile_size() / 2.0f));
    GameScene::entity_manager.list.push_back(this->player);

    // spawnTimer / spawnDelay already default-initialized in header (if not, set here)
    spawnTimer = 0.f;
    spawnDelay = 2.f;
}

void GameScene::spawn_enemy() {
    Enemy e;
    e.shape = sf::CircleShape(40.f);
    e.shape.setFillColor(sf::Color::Green);
    e.shape.setOrigin(sf::Vector2f(40.f, 40.f));

    // FALLBACK: place enemy at LevelSystem start pos
    static std::vector<sf::Vector2f> dummyPath;
    if (!dummyPath.size()) {
        // fallback single-node path (replace with real path)
        dummyPath.push_back(LevelSystem::get_start_position());
    }
    e.path = &dummyPath;
    e.shape.setPosition((*e.path)[0]);

    enemies.push_back(e);
}

void GameScene::update_enemies(float dt) {
    for (auto& e : enemies) {
        if (!e.path || e.pathIndex >= e.path->size()) continue;

        sf::Vector2f target = (*e.path)[e.pathIndex];
        sf::Vector2f pos = e.shape.getPosition();
        sf::Vector2f dir = target - pos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (len < 2.f) {
            // arrived at node -> advance
            e.pathIndex++;
            continue;
        }

        sf::Vector2f norm = dir / len;
        e.shape.move(norm * e.speed * dt);
    }
}

bool GameScene::can_place_turret(sf::Vector2f mousePos) {
    // convert pixel/world position to grid
    float tileSize = LevelSystem::get_tile_size();
    int col = static_cast<int>(mousePos.x / tileSize);
    int row = static_cast<int>(mousePos.y / tileSize);

    // boundary check
    if (row < 0 || col < 0 ||
        row >= static_cast<int>(LevelSystem::get_height()) ||
        col >= static_cast<int>(LevelSystem::get_width()))
        return false;

#ifdef LEVELSYSTEM_HAS_GET_TILE
    return LevelSystem::get_tile(row, col) == 'w';
#else
// FALLBACK: allow placement anywhere for now
    return true;
#endif
}

void GameScene::place_turret(sf::Vector2f mousePos) {
    Tower t;
    t.shape = sf::CircleShape(25.f);
    t.shape.setFillColor(sf::Color::Blue);
    t.shape.setOrigin(sf::Vector2f(25.f, 25.f));

    // snap to grid
    float tile = LevelSystem::get_tile_size();
    int col = static_cast<int>(mousePos.x / tile);
    int row = static_cast<int>(mousePos.y / tile);
    sf::Vector2f snappedPos(col * tile + tile / 2.f, row * tile + tile / 2.f);

    t.pos = snappedPos;
    t.shape.setPosition(snappedPos);

    towers.push_back(t);
}

// ---------- single, correct update method (must match header) ----------
void GameScene::update(const float& delta_time) {
    // keep base behaviour
    Scene::update(delta_time);

    // --- spawn logic ---
    spawnTimer += delta_time;
    if (spawnTimer >= spawnDelay) {
        spawnTimer = 0.f;
        spawn_enemy();
    }

    // --- update enemies ---
    update_enemies(delta_time);

    // --- mouse handling / hover / placement ---
    sf::Vector2f mousePos;
#ifdef RENDERER_HAS_WINDOW_PTR
    sf::Vector2i pixelPos = sf::Mouse::getPosition(*Renderer::get_window());
    mousePos = Renderer::get_window()->mapPixelToCoords(pixelPos);
#else
    // FALLBACK: raw mouse coords (window is fullscreen)
    sf::Vector2i pixelPos = sf::Mouse::getPosition();
    mousePos = sf::Vector2f(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
#endif

    // hover preview (example using a preview shape)
    sf::CircleShape previewShape(LevelSystem::get_tile_size() * 0.5f);
    previewShape.setOrigin(sf::Vector2f(previewShape.getRadius(), previewShape.getRadius()));
    previewShape.setPosition(mousePos);
    previewShape.setFillColor(can_place_turret(mousePos) ? sf::Color(0, 255, 0, 120) : sf::Color(255, 0, 0, 120));
    Renderer::queue(&previewShape); // queue for rendering preview

    // left-click placement
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (can_place_turret(mousePos)) {
            place_turret(mousePos);
        }
    }
}

void GameScene::render() {
    LevelSystem::render();

    // draw towers
    for (auto& t : towers) {
        Renderer::queue(&t.shape);
    }

    // draw enemies
    for (auto& e : enemies) {
        Renderer::queue(&e.shape);
    }

    Scene::render();
}
