#include "TowerMenu.h"

TowerMenu::TowerMenu() {
    float buttonSize = 60.f;
    float spacing = 10.f;
    float x = 10.f;
    float y = 10.f;

    towerInfos = {
        { "Basic Tower",  &BASIC_TOWER,  50 },
        { "Sniper Tower", &SNIPER_TOWER, 100 },
        { "Cannon Tower", &CANNON_TOWER, 150 }
    };

    for (size_t i = 0; i < towerInfos.size(); ++i) {
        auto& info = towerInfos[i];
        info.button.setSize({ buttonSize, buttonSize });
        info.button.setPosition(x, y + i * (buttonSize + spacing));
        info.button.setFillColor(sf::Color::Blue);
    }
}

void TowerMenu::render(sf::RenderWindow& window) {
    for (auto& info : towerInfos)
        window.draw(info.button);
}

void TowerMenu::handleClick(sf::Vector2f mousePos) {
    for (auto& info : towerInfos) {
        if (info.button.getGlobalBounds().contains(mousePos)) {
            selectedTower = info.stats;
            selectedTowerCost = info.cost;
            std::cout << info.name << " selected! Cost: " << selectedTowerCost << "\n";
        }
    }
}

const TowerStats* TowerMenu::getSelectedTower() const {
    return selectedTower;
}