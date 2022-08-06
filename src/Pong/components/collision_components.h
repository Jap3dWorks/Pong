//
// Created by Jordi on 6/26/2022.
//

#ifndef GL_TEST_COLLISION_COMPONENTS_H
#define GL_TEST_COLLISION_COMPONENTS_H

#include "Pong/components/component.h"
#include "Pong/core/collider.h"
#include "Pong/core/actor/actor.h"

namespace Pong {
// collision components
    class CollisionComponent : public Pong::Component {
    public:
        ~CollisionComponent() override { LOG_DEBUG("Collision Component destructor"); }

        virtual void at_collision(Pong::Collider *owner, Pong::Collider *other) = 0;
    };

    class BallCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Pong::Collider *owner, Pong::Collider *other) override {
            // Get ball Actor
            auto *ball = static_cast<Pong::ABall *>(owner->actor);
            auto *sphere_coll = static_cast<Pong::SphereCollider *>(owner);

            const Pong::CollisionData *coll_data = other->get_collision_data();
            // move ball out of collition zone
            glm::vec3 ball_pnt = ball->transform[3];

            auto collision_distance = glm::vec3(ball_pnt - coll_data->point);

            glm::vec3 radius_vec = coll_data->normal * sphere_coll->getRadius();
            LOG_DEBUG("--" + other->getName() + "--");

            LOG_DEBUG("Normal collision: " << coll_data->normal.x << ", " <<
                                           coll_data->normal.y << "," << coll_data->normal.z);

            LOG_DEBUG("Point collision: " << coll_data->point.x << ", " <<
                                          coll_data->point.y << ", " << coll_data->point.z);

            LOG_DEBUG("id collision: " << coll_data->face_id);

            // add 0.01 normal value, to avoid double collisions
            glm::vec3 relocate_position = collision_distance - radius_vec + coll_data->normal * 0.05f;

            // multiply ajust vector by 1.5 to avoid double collisions.
            ball->transform = glm::translate(ball->transform, relocate_position);

            // reflect _direction
            glm::vec3 collide_direction = glm::reflect(ball->getDirection(), coll_data->normal);

            ball->set_direction(collide_direction);

            if (auto other_actor = dynamic_cast<Pong::APlayer *>(other->actor)) {
                if (other_actor->getVelocity()) {
                    collide_direction += other_actor->get_vector_director() * 30.f;  // player affecting ball _direction
                }
                ball->setVelocity(ball->getVelocity() + 0.5);
            }

            ball->set_direction(collide_direction);
        }

        ~BallCollisionComponent() override {
            LOG_DEBUG("BallCollisionComponent destructor");
        }
    };

    class BorderCollisionComponent : public CollisionComponent {
    public:
        void at_collision(Pong::Collider *owner, Pong::Collider *other) override  {
            Pong::ABall *ball = dynamic_cast<Pong::ABall *>(other->getActor());

            if (ball) {
                glm::mat4 b_trnsform = ball->transform;
                b_trnsform[3] = glm::vec4(0, 0, 0, 1);
                ball->transform = b_trnsform;

                // set a random _direction
                srand(time(nullptr));
                const double pi = 3.14159265358979323846;
                float rand_angle = 2.f * pi * (rand() % 100) / 100.f;

                float sin_val = sin(rand_angle);
                float cos_val = cos(rand_angle);

                ball->set_direction(glm::vec3(cos_val, sin_val, 0));
                ball->setVelocity(2.f);
            }
        }

        ~BorderCollisionComponent() override {
            LOG_DEBUG("BorderCollisionComponent destructor");
        }
    };
}

#endif //GL_TEST_COLLISION_COMPONENTS_H
