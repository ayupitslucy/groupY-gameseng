#pragma once
#include <SFML/Graphics.hpp>

namespace Renderer {
    void initialise(sf::RenderWindow& render_window);
    
    sf::RenderWindow& get_window();

    void queue(const sf::Drawable* sprite);
    void render();
    void shutdown();

    static sf::RenderWindow* render_window;
};