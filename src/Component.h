#ifndef COMPONENT_H
#define COMPONENT_H

//namespace Pong
//{
//    class Actor;
////    class Collider;
//}

#include "Actor.h"
#include "Collider.h"
#include "logger.h"

#include <iostream>

namespace Pong {
    /**
        Component class, an actor has a list of compponents, each component
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
    class CollisionComponent: public Component
    {
    public:
        virtual ~CollisionComponent() {
            LOG_DEBUG("Collision component destructor");
        }

        Collider* collider = nullptr;
        virtual void at_collision(Collider* other) = 0;
    };


    class BallCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Collider *other) override;

        virtual ~BallCollisionComponent() override {
            LOG_DEBUG("BallCollisionComponent destructor");
        }
    };

    class BorderCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Collider *other) override;

        virtual ~BorderCollisionComponent() override {
            LOG_DEBUG("BorderCollisionComponent destructor");
        }
    };

    // actor components
    class ActorComponent
    {
    public:
        virtual ~ActorComponent()
        {
            LOG_DEBUG("Actor component destructor");
        }
        Actor* actor = nullptr;
    };
}
#endif // COMPONENT_H