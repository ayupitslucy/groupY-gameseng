#include "entity_component_system.h"

// ================================================================ Entity ================================================================

Entity::Entity() {
    Entity::alive = true;
    Entity::for_deletion = false;
    Entity::visible = true;
}

Entity::~Entity() {}

void Entity::update(const float& delta_time) {
    for (std::shared_ptr<Component> component : Entity::components) {
        component->update(delta_time);
    }
}

void Entity::render() {
    for (std::shared_ptr<Component> component : Entity::components) {
        component->render();
    }
}

const sf::Vector2f& Entity::get_position() const {
    return this->position;
}

void Entity::set_position(const sf::Vector2f& position) {
    this->position = position;
}

bool Entity::is_for_deletion() const {
    return this->for_deletion;
}

float Entity::get_rotation() const {
    return this->rotation;
}

void Entity::set_rotation(float rotation) {
    this->rotation = rotation;
}

bool Entity::is_alive() const {
    return this->alive;
}

void Entity::set_alive(bool alive) {
    this->alive = alive;
}

void Entity::set_for_delete() {
    this->for_deletion = true;
}

bool Entity::is_visible() const {
    return this->visible;
}

void Entity::set_visible(bool visible) {
    this->visible = visible;
}

// ================================================================ Component ================================================================

bool Component::is_for_deletion() const {
    return this->for_deletion;
}

Component::~Component() {}

Component::Component(Entity* const parent) : parent(parent) {}