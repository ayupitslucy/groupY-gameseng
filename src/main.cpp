#include "scenes.h"
#include "game_system.h"
#include "sound.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Scenes");
    Scene* currentScene = new MenuScene();

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            GameSystem::handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        GameSystem::update(dt);

        window.clear();
        GameSystem::draw(window);
        window.display();
    }

    initBackgroundMusic();
    delete currentScene;
    return 0;
}