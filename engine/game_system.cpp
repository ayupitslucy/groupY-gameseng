#include "game_system.h"
#include "renderer.h"

std::shared_ptr<Scene> GameSystem::active_scene;

void GameSystem::start(unsigned int width, unsigned int height, const std::string& name, const float& time_step) {
    sf::RenderWindow window(sf::VideoMode({ width, height }), name);
    Renderer::initialise(window);
    sf::Event event;
    while (window.isOpen()) {
        static sf::Clock clock;
        float delta_time = clock.restart().asSeconds();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                clean();
                return;
            }
        }
        window.clear();
        GameSystem::_update(delta_time);
        GameSystem::_render();
        Renderer::render();
        sf::sleep(sf::seconds(time_step));
        //Wait for Vsync
        window.display();
    }
    window.close();
    clean();
}

void GameSystem::clean() {
    GameSystem::active_scene->unload();
}

void GameSystem::set_active_scene(const std::shared_ptr<Scene>& active_scene) {
    GameSystem::active_scene = active_scene;
}

std::shared_ptr<Scene> GameSystem::get_active_scene() {
    return GameSystem::active_scene;
}

void GameSystem::_update(const float& delta_time) {
    GameSystem::active_scene->update(delta_time);
}

void GameSystem::_render() {
    GameSystem::active_scene->render();
}
