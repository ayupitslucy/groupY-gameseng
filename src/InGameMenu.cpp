//#include "InGameMenu.h"
//#include "engine/game_system.h"

//InGameMenu::InGameMenu(const sf::Font& font)
//{
//    background.setSize({ 800.f, 600.f });
//    background.setFillColor(sf::Color(0, 0, 0, 150));
//
//    buttons.push_back(std::make_shared<Button>(
//        sf::Vector2f(325.f, 200.f), sf::Vector2f(150.f, 50.f),
//        "Resume", font, [this]() { close(); }
//    ));

//    buttons.push_back(std::make_shared<Button>(
//        sf::Vector2f(325.f, 300.f), sf::Vector2f(150.f, 50.f),
//        "Quit", font, []() {
//            GameSystem::requestQuit();
//        }
//    ));
//}

//void InGameMenu::addButton(...) { /* optional extra buttons */ }

//void InGameMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window)
//{
//    if (!menuOpen) return;
//
//    for (auto& btn : buttons)
//        btn->handleEvent(event, window);
//}

//void InGameMenu::render(sf::RenderWindow& window)
//
//    if (!menuOpen) return;

//    window.draw(background);
//    for (auto& btn : buttons)
//        btn->render(window);
//}