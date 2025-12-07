#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    // Constructor
    Button(const sf::Vector2f& position, const sf::Vector2f& size,
        const std::string& labelStr, sf::Font& font,
        std::function<void()> callbackFunc,
        const std::string& iconPath = "");

    // Draw button
    void render(sf::RenderWindow& window);

    // Handle clicks
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    // Optional: change color on hover
    void updateHover(const sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Texture iconTexture;
    sf::Sprite iconSprite;
    bool iconLoaded = false;

    std::function<void()> callback;
};