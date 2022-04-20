#include "Core/Component.h"
#include <time.h>

namespace Pong {
    void BallCollisionComponent::at_collision(Collider *owner, Collider *other)
    {
        // Get ball actor
        auto* ball = static_cast<ABall*>(owner->actor);
        auto* sphere_coll = static_cast<SphereCollider*>(owner);

        const CollisionData* coll_data = other->get_collision_data();
        // move ball out of collition zone
        glm::vec3 ball_pnt = ball->get_transform()[3];

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
        ball->set_transform(glm::translate(ball->get_transform(), relocate_position));

        // reflect _direction
        glm::vec3 collide_direction = glm::reflect(ball->getDirection(), coll_data->normal);

        ball->set_direction(collide_direction);

        if (auto other_actor = dynamic_cast<APlayer *>(other->actor)) {
            if (other_actor->getVelocity()) {
                collide_direction += other_actor->get_vector_director() * 30.f;  // player affecting ball _direction
            }
            ball->setVelocity(ball->getVelocity() + 0.5);
        }

        ball->set_direction(collide_direction);
    }

    // BorderCollisionComponent
    // ------------------------
    void BorderCollisionComponent::at_collision(Collider *owner, Collider *other)
    {
        ABall* ball = dynamic_cast<ABall*>(other->getActor());

        if (ball)
        {
            glm::mat4 b_trnsform = ball->get_transform();
            b_trnsform[3] = glm::vec4(0, 0, 0, 1);
            ball->set_transform(b_trnsform);

            // set a random _direction
            srand(time(nullptr));
            const double pi = 3.14159265358979323846;
            float rand_angle =  2.f * pi * (rand() % 100) / 100.f;

            float sin_val = sin(rand_angle);
            float cos_val = cos(rand_angle);

            ball->set_direction(glm::vec3(cos_val, sin_val, 0));
            ball->setVelocity(2.f);
        }
    }
}