#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pong/core/actor.h"
#include "Pong/core/collider.h"
#include "Pong/logger.h"
#include <typeinfo>
#include <cassert>

#include <iostream>

namespace Pong {
    /**
        Component class, an Actor has a list of compponents, each Component
        can contain a custom script you only have to inherit from Component.
    */
    class Component {
    public:
        virtual ~Component() {}

        virtual void at_init(Actor* actor) {}
        virtual void at_frame(Actor* actor) {}
    };

    class ComponentMap {
        using ComponentData = std::map<size_t, std::vector<Component*>>;
        ComponentData _component_map;

    public:
        ComponentMap()=default;
        ~ComponentMap()=default;

        ComponentMap(ComponentMap&& other) noexcept {
            std::swap(_component_map, other._component_map);
        }

        ComponentMap& operator=(ComponentMap&& other) noexcept {
            _component_map = std::move(other._component_map);
        }

        ComponentMap(const ComponentMap& other) = delete;
        ComponentMap& operator=(const ComponentMap& other) = delete;

    public:
        template<typename T>
        void register_type() {
            auto t_info = typeid(T);
            auto type_hash = t_info.hash_code();
            _component_map[type_hash] = std::vector<Component*>;
        }

        template<typename T>
        bool contains_type() {
            auto t_info = typeid(T);
            return _component_map.contains(t_info.hash_code());
        }

        template<typename T>
        std::vector<Component*>& get_components() {
            auto t_info = typeid(T);
            assert(_component_map.contains(t_info.hash_code()));

            return _component_map[t_info.hash_code()];
        }

        template<typename T>
        void add_component(T* component) {
            auto t_info = typeid(T);
            _component_map[t_info.hash_code()].push_back(component);
        }

        const ComponentData& component_map() {
            return _component_map;
        }
    };

}
#endif // COMPONENT_H