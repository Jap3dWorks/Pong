//
// Created by Jordi on 7/17/2020.
//

#ifndef PONG_DATACOMPARERS_H
#define PONG_DATACOMPARERS_H

#include "../logger.h"
#include "Actor.h"

namespace Pong {
    template<typename T>
    struct NameComparer {
    public:
        bool operator()(const T& first, const T& second) const {
            return first.get_name() < second.get_name();
        }
    };

    template<typename T>
    struct NameComparer<T*> {
    public:
        bool operator()(const T* first, const T* second) const {
            return first->get_name() < second->get_name();
        }
    };

    template<typename T>
    struct OrderComparer {
    public:
        bool operator()(const T& first, const T& second) const {
            return first.order < second.order;
        }
    };

    template<typename T>
    struct OrderComparer<T*> {
    public:
        bool operator()(const T* first, const T* second) const {
            return first->order < second->order;
        }
    };

    template<typename T>
    bool compare_less_order(T* first, T* second)
    {
        return first->order < second->order;
    }

    struct ActorCameraDistanceComparer {
        const ACamera *camera_ptr = nullptr;

        explicit ActorCameraDistanceComparer(const ACamera *camera) : camera_ptr(camera) {}

        [[nodiscard]] inline float get_distance(const glm::mat4 &mat_a) const {
            return glm::length<3, float>(glm::vec3(mat_a[3]) - camera_ptr->Position);
        }

        bool operator()(const Actor &act_a, const Actor &act_b) const {
            return get_distance(act_a.get_transform()) < get_distance(act_b.get_transform());
        }

        bool operator()(const Actor * act_a, const Actor * act_b) const {
//            LOG_DEBUG("Compare " << act_a->get_name() << " and " << act_b->get_name());
//            LOG_DEBUG(act_a->get_name() << " " << act_a->get_transform()[3][0] << " " << act_a->get_transform()[3][1] << " " << act_a->get_transform()[3][2]);
//            LOG_DEBUG(act_b->get_name() << " " << act_b->get_transform()[3][0] << " " << act_b->get_transform()[3][1] << " " << act_b->get_transform()[3][2]);
//            LOG_DEBUG("Camera " << camera_ptr->get_name());
//            LOG_DEBUG("Camera pos: ");
//            LOG_DEBUG(act_a->get_name() << " " << get_distance(act_a->get_transform()));
//            LOG_DEBUG(act_b->get_name() << " " << get_distance(act_b->get_transform()));

            bool result = get_distance(act_a->get_transform()) < get_distance(act_b->get_transform());

//            LOG_DEBUG(result);

            return result;
        }

    };
}
#endif //PONG_DATACOMPARERS_H
