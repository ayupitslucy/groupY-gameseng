// The entire contents of this file are temporary
#include <SFML/Graphics.hpp>
#include "engine/tile_engine.h"
#include "engine/renderer.h"

int main(int argc, char** argv) {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project");
    window.setFramerateLimit(144);
    Renderer::initialise(window);
    LevelSystem::load_level("res/levels/temp_level.txt", 150.0f);
    sf::CircleShape shape(75.0f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(LevelSystem::get_start_position());

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                Renderer::shutdown();
            }
        }

        window.clear();
        LevelSystem::render();
        Renderer::render();
        window.draw(shape);
        window.display();
    }
    return 0;
}