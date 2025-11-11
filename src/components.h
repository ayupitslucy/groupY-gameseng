#pragma once
#include "engine/entity_component_system.h"

class ShapeComponent : public Component {
public:
    ShapeComponent() = delete;
    explicit ShapeComponent(Entity* const parent);

    void update(const float& delta_time) override;
    void render() override;

    sf::Shape& get_shape() const;
    std::shared_ptr<sf::Shape> get_shape_ptr() const;

    template <typename T, typename... Targs> void set_shape(Targs... params) {
        shape.reset(new T(params...));
    }
protected:
    std::shared_ptr<sf::Shape> shape;
};