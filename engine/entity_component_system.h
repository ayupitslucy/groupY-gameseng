#pragma once
#include <SFML/Graphics.hpp>

class Component;

class Entity {
public:
    Entity();
    virtual ~Entity();
    virtual void update(const float& delta_time);
    virtual void render();

    const sf::Vector2f& get_position() const;
    void set_position(const sf::Vector2f& position);
    bool is_for_deletion() const;
    sf::Angle get_rotation() const;
    void set_rotation(sf::Angle rotation);
    bool is_alive() const;
    void set_alive(bool alive);
    void set_for_delete();
    bool is_visible() const;
    void set_visible(bool visible);

    template <typename T, typename... Targs> std::shared_ptr<T> add_component(Targs... params) {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::shared_ptr<T> component(std::make_shared<T>(this, params...));
        components.push_back(component);
        return component;
    }

    template <typename T> const std::vector<std::shared_ptr<T>> get_components() const {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> found_components;
        for (const auto component : this->components) {
            if (typeid(*component) == typeid(T)) {
                found_components.push_back(std::dynamic_pointer_cast<T>(component));
            }
        }
        return std::move(found_components);
    }

    template <typename T> const std::vector<std::shared_ptr<T>> get_compatible_components() {
        static_assert(std::is_base_of<Component, T>::value, "T != component");
        std::vector<std::shared_ptr<T>> found_components;
        for (auto component : this->components) {
            auto derived_component_ptr = dynamic_cast<T*>(&(*component));
            if (derived_component_ptr) {
                found_components.push_back(std::dynamic_pointer_cast<T>(component));
            }
        }
        return found_components;
    }
protected:
    std::vector<std::shared_ptr<Component>> components;
    sf::Vector2f position;
    sf::Angle rotation;
    bool alive;  // Should be updated
    bool visible;  // Should be rendered
    bool for_deletion;  // Should be deleted
};

class Component {
public:
    Component() = delete;
    bool is_for_deletion() const;
    virtual void update(const float& delta_time) = 0;
    virtual void render() = 0;
    virtual ~Component();
protected:
    Entity* const parent;
    bool for_deletion;  // Should be removed
    explicit Component(Entity* const parent);
};