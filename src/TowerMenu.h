#pragma once
#include "game_entities.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>

struct TowerButtonInfo {
    std::string name;
    const TowerStats* stats;
    int cost;
    sf::RectangleShape button;
};

class TowerMenu {
public:
    TowerMenu();
    void render(sf::RenderWindow& window);
    void handleClick(sf::Vector2f mousePos);
    const TowerStats* getSelectedTower() const;
    int getSelectedTowerCost() const { return selectedTowerCost; }

private:
    std::vector<TowerButtonInfo> towerInfos;
    const TowerStats* selectedTower = nullptr;
    int selectedTowerCost = 0;
};