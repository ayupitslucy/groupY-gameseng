#include "renderer.h"
#include <queue>

static std::queue<const sf::Drawable*> sprites;

void Renderer::initialise(sf::RenderWindow& render_window) {
    Renderer::render_window = &render_window;
}

sf::RenderWindow& Renderer::get_window() {
    return *Renderer::render_window;
}

void Renderer::queue(const sf::Drawable* sprite) {
    sprites.push(sprite);
}

void Renderer::render() {
    if (Renderer::render_window == nullptr) {
        throw std::runtime_error("No render window set!");
    }
    while (!sprites.empty()) {
        const sf::Drawable* d = sprites.front();
        if (!d) {
            throw std::runtime_error("Renderer::render(): nullptr in queue");
        }
        Renderer::render_window->draw(*d);
        sprites.pop();
    }
}

void Renderer::shutdown() {
    while (!sprites.empty()) {
        sprites.pop();
    }
}