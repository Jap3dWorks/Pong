#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pong/core/actor.h"
#include "Pong/core/collider.h"
#include "utils/subclasses_map.h"
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

}

#endif // COMPONENT_H