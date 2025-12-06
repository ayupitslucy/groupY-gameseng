#include "Button.h"
#include <iostream>

// Constructor
Button::Button(const sf::Vector2f& position, const sf::Vector2f& size,
    const std::string& labelStr, sf::Font& font,
    std::function<void()> callbackFunc,
    const std::string& iconPath)
    : callback(callbackFunc)
{
    // Configure shape
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color(100, 100, 100)); // default grey

    // Configure text
    text.setFont(font);
    text.setString(labelStr);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    // Center text inside button
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);
    text.setPosition(position + size / 2.f);

    // Load icon if provided
    if (!iconPath.empty()) {
        if (!iconTexture.loadFromFile(iconPath)) {
            std::cerr << "Failed to load icon: " << iconPath << std::endl;
            iconLoaded = false;
        }
        else {
            iconSprite.setTexture(iconTexture);
            // Scale icon to fit button size (optional)
            sf::Vector2f scale(
                size.x / iconTexture.getSize().x,
                size.y / iconTexture.getSize().y
            );
            iconSprite.setScale(scale);
            iconSprite.setPosition(position);
            iconLoaded = true;
            std::cerr << "Loaded icon: " << iconPath << std::endl;
        }
    }
}

// Draw button
void Button::render(sf::RenderWindow& window)
{
    window.draw(shape);
    window.draw(text);
    if (iconLoaded)
        window.draw(iconSprite);
}

// Handle clicks
void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
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

// Hover effect
void Button::updateHover(const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (shape.getGlobalBounds().contains(mousePos))
        shape.setFillColor(sf::Color(150, 150, 150)); // hover grey
    else
        shape.setFillColor(sf::Color(100, 100, 100)); // default grey
}