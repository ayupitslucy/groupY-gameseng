#include "scenes.h"
#include <SFML/Graphics.hpp>
#include "engine/game_system.h"
#include "engine/renderer.h"
#include "engine/tile_engine.h"
#include "game_parameters.h"
#include "components.h"

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
    this->font = std::make_unique<sf::Font>("res/fonts/RobotoMono-Regular.ttf");
    this->text = std::make_unique<sf::Text>(*(this->font), "Menu Screen (Press space to play)");
    this->text->setCharacterSize(24);
    this->text->setPosition(sf::Vector2f((Parameters::game_width * .5f) - (this->text->getLocalBounds().size.x * .5f), 0));
}

// ================================ GameScene ================================

void GameScene::update(const float& delta_time) {
    Scene::update(delta_time);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
        GameSystem::set_active_scene(Scenes::menu);
    }
}

void GameScene::render() {
    LevelSystem::render();
    Scene::render();
}
void GameScene::load() {
    LevelSystem::load_level(Parameters::level_1, 100.0f);

    this->player = std::make_shared<Entity>();
    std::shared_ptr<ShapeComponent> shape = this->player->add_component<ShapeComponent>();
    shape->set_shape<sf::CircleShape>(50.0f);
    shape->get_shape().setFillColor(sf::Color::Red);
    shape->get_shape().setOrigin(sf::Vector2f(50.0f, 50.0f));
    this->player->set_position(LevelSystem::get_start_position() + sf::Vector2f(LevelSystem::get_tile_size() / 2.0f, LevelSystem::get_tile_size() / 2.0f));
    GameScene::entity_manager.list.push_back(this->player);
}