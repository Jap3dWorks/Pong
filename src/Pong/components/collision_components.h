//
// Created by Jordi on 6/26/2022.
//

#ifndef GL_TEST_COLLISION_COMPONENTS_H
#define GL_TEST_COLLISION_COMPONENTS_H

#include "Pong/components/component.h"
#include "Pong/core/collider.h"
#include "Pong/core/reg_id_manager.h"
// #include "Pong/core/actor/actor.h"

namespace Pong {
// collision components
    class CollisionComponent : public Pong::Component {
    public:
        RegId collider;

        //        virtual void at_collision(Pong::Collider *owner, Pong::Collider *other) = 0;
    };
}

#endif //GL_TEST_COLLISION_COMPONENTS_H
