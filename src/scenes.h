#pragma once
#include <memory>
#include "engine/scene.h"

struct Scenes {
    static std::shared_ptr<Scene> menu;
    static std::shared_ptr<Scene> game;
};

class MenuScene : public Scene {
public:
    MenuScene() = default;
    void update(const float& delta_time) override;
    void render() override;
    void load() override;
private:
    std::unique_ptr<sf::Text> text;
    std::unique_ptr<sf::Font> font;
};

class GameScene : public Scene {
public:
    GameScene() = default;
    void update(const float& delta_time) override;
    void render() override;
    void load() override;
private:
    std::shared_ptr<Entity> player;
};