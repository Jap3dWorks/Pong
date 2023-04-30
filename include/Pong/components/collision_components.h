//
// Created by Jordi on 6/26/2022.
//

#ifndef GL_TEST_COLLISION_COMPONENTS_H
#define GL_TEST_COLLISION_COMPONENTS_H

#include "Pong/components/component.h"
#include "Pong/core/collider.h"
#include "Pong/registers/reg_id.h"
// #include "Pong/core/entity/entity.h"

namespace pong {
// collision components
    class CollisionComponent : public pong::Component {
    public:
        RegId collider;

        //        virtual void at_collision(pong::.Collider *owner, pong::.Collider *other) = 0;
    };
}

#endif //GL_TEST_COLLISION_COMPONENTS_H
