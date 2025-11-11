#include "scene.h"

void Scene::update(const float& delta_time) {
    for (std::shared_ptr<Entity>& entity : entity_manager.list) {
        entity->update(delta_time);
    }

    // Remove all entities marked for deletion
    auto new_end = std::remove_if(
        entity_manager.list.begin(),
        entity_manager.list.end(),
        [](const std::shared_ptr<Entity>& entity) {
            return entity->is_for_deletion();
        }
    );
    entity_manager.list.erase(new_end, entity_manager.list.end());
}

void Scene::render() {
    for (std::shared_ptr<Entity>& entity : entity_manager.list) {
        entity->render();
    }
}

void Scene::unload() {}