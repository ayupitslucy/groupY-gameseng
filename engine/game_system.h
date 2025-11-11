#pragma once
#include <string>
#include <memory>
#include "scene.h"

class GameSystem {
public:
    static void start(unsigned int width, unsigned int height, const std::string& name, const float& time_step);  // Start the game
    static void clean();
    static void set_active_scene(const std::shared_ptr<Scene>& active_scene);
    static std::shared_ptr<Scene> get_active_scene();

private:
    static void _update(const float& delta_time);
    static void _render();
    static std::shared_ptr<Scene> active_scene;
};