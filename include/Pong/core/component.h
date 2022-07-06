#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pong/core/actor.h"
#include "Pong/core/collider.h"
#include "utils/subclasses_map.h"
#include "Pong/logger.h"
#include <vector>
#include <cassert>

#include <iostream>

namespace Pong {
    /**
        Component class, an Actor has a list of components, each Component
        can contain a custom script you only have to inherit from Component.
    */
    class Component {
    protected:
        using component_super = Component;

    private:
        std::vector<Component*> _components;
    public:
        Component()=default;
        virtual ~Component()=default;

        virtual void start(Actor* actor, Component* parent=nullptr) {
            for(auto& comp: _components) {
                comp->start(actor, this);
            }
        }

        virtual void by_frame(Actor* actor, Component* parent) {
            for (auto& comp: _components) {
                comp->by_frame(actor, this);
            }
        }
    };

    using ComponentMap = SubClassMap<Component>;
}

#endif // COMPONENT_H