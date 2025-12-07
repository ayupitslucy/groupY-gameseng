//#pragma once
//#include <SFML/Graphics.hpp>
//#include <memory>
//#include <vector>
//#include "button.h"

//class InGameMenu {
//public:
//    InGameMenu(const sf::Font& font);

//    void addButton(const std::string& label, std::function<void()> callback, sf::Vector2f pos);
//    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
//    void render(sf::RenderWindow& window);

//    void open() { menuOpen = true; }
//    void close() { menuOpen = false; }
//    bool isOpen() const { return menuOpen; }

//private:
//    bool menuOpen = false;
//    std::vector<std::shared_ptr<Button>> buttons;
//    sf::RectangleShape background;
//};