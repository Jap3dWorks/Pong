#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pong/core/actor.h"
#include "Pong/core/collider.h"
#include "Pong/logger.h"

#include <iostream>

namespace Pong {
    /**
        Component class, an Actor has a list of compponents, each Component
        can contain a custom script you only have to inherit from Component.
    */
    class Component
    {
    public:
        virtual ~Component() {}

        virtual void at_init() {}
        virtual void each_frame() {}
    };

    // collision components
    class CollisionComponent: public Component {
    public:
        virtual ~CollisionComponent() { LOG_DEBUG("Collision Component destructor"); }

        virtual void at_collision(Collider *owner, Collider *other) = 0;
    };


    class BallCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Collider *owner, Collider *other) override;

        virtual ~BallCollisionComponent() override {
            LOG_DEBUG("BallCollisionComponent destructor");
        }
    };

    class BorderCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Collider *owner, Collider *other) override;

        virtual ~BorderCollisionComponent() override {
            LOG_DEBUG("BorderCollisionComponent destructor");
        }
    };

    // Actor components
    class ActorComponent : public Component {
    public:
        virtual ~ActorComponent() {
            LOG_DEBUG("Actor Component destructor");
        }

        virtual void at_init(Actor*& owner) = 0;
        virtual void each_frame(Actor*& owner) = 0;
    };
}
#endif // COMPONENT_H