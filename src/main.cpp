// The entire contents of this file are temporary
#include <SFML/Graphics.hpp>

int main(int argc, char** argv) {
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "CMake SFML Project");
    window.setFramerateLimit(144);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
    return 0;
}