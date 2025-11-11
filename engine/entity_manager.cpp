#include "entity_manager.h"

void EntityManager::update(float delta_time) {
    for (std::shared_ptr<Entity> entity : EntityManager::list) {
        entity->update(delta_time);
    }
}

void EntityManager::render() {
    for (std::shared_ptr<Entity> entity : EntityManager::list) {
        entity->render();
    }
}