#include "components.h"
#include "engine/renderer.h"

ShapeComponent::ShapeComponent(Entity* const parent) : Component(parent), shape(std::make_shared<sf::CircleShape>()) {}

void ShapeComponent::update(const float& delta_time) {
    shape->setPosition(parent->get_position());
    shape->setRotation(parent->get_rotation());
}
void ShapeComponent::render() {
    Renderer::queue(shape.get());
}

sf::Shape& ShapeComponent::get_shape() const {
    return *shape;
}
std::shared_ptr<sf::Shape> ShapeComponent::get_shape_ptr() const {
    return shape;
}