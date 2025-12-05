#include "scenes.h"
#include "game_system.h"
//#include <SFML/Audio.hpp>
#include "renderer.h"

//#include <iostream>

int main() {
    // Create the window first
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Scenes");

    // Initialise renderer immediately
    Renderer::initialise(window);

    // Create scenes (ok to create before or after as long as Renderer is initialised
    Scenes::menu = std::make_shared<MenuScene>();
    Scenes::menu->load();
    Scenes::game = std::make_shared<GameScene>();
    Scenes::game->load();

    GameSystem::setActiveScene(Scenes::menu);

    //sf::Music music;
    //if (!music.openFromFile("Media/__rhodesmas__music-loop.wav")) {
    //    std::cerr << "Failed to load music\n";
    //    return 1;
    //}
    //music.setLoop(true);
    //music.play();


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

    return 0;
}