#pragma once
#include <vector>
#include <memory>
#include "entity_manager.h"
#include "entity_component_system.h"

class Scene {
public:
    Scene() = default;
    virtual void load() = 0;
    std::vector<std::shared_ptr<Entity>>& get_entities() { return entity_manager.list; }

    virtual void update(const float& delta_time);  // Update all entities in the scene
    virtual void render();  // Render all entities in the scene

    virtual void unload();
    virtual ~Scene() = default;

protected:
    EntityManager entity_manager;
};