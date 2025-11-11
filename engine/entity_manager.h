#pragma once
#include <vector>
#include <memory>
#include "entity_component_system.h"

struct EntityManager {
    std::vector<std::shared_ptr<Entity>> list;
    void update(float delta_time);
    void render();
};