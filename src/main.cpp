#include "engine/game_system.h"
#include "game_parameters.h"
#include "scenes.h"

int main() {
    Scenes::menu = std::shared_ptr<MenuScene>(new MenuScene());
    Scenes::menu->load();
    Scenes::game = std::make_shared<GameScene>();
    Scenes::game->load();
    GameSystem::set_active_scene(Scenes::menu);
    GameSystem::start(Parameters::game_width, Parameters::game_height, "Foodies", Parameters::time_step);
    return 0;
}