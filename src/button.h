#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& textStr,
        sf::Font& font, std::function<void()> onClick)
        : callback(onClick)
    {
        // Configure shape
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color(100, 100, 100)); // default grey

        // Configure text
        text.setFont(font);
        text.setString(textStr);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);

        // Center text inside button
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        text.setPosition(position + size / 2.f);
    }

    // Draw button
    void render(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    // Handle clicks
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                { event.mouseButton.x, event.mouseButton.y });
            if (shape.getGlobalBounds().contains(mousePos)) {
                if (callback) callback();
            }
        }
    }

    // Optional: change color on hover
    void updateHover(const sf::RenderWindow& window) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (shape.getGlobalBounds().contains(mousePos))
            shape.setFillColor(sf::Color(150, 150, 150)); // hover grey
        else
            shape.setFillColor(sf::Color(100, 100, 100)); // default grey
    }

private:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> callback;
};