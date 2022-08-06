//
// Created by Jordi on 7/17/2020.
//

#ifndef PONG_DATACOMPARERS_H
#define PONG_DATACOMPARERS_H

#include "Pong/core/actor/actor.h"
#include "Pong/core/core_vals.h"
#include "Utils/logger.h"

namespace Pong {
//    template<typename Derived_>
//    struct NameComparer {
//    public:
//        bool operator()(const Derived_& first, const Derived_& second) const {
//            return first.get_name() < second.get_name();
//        }
//    };
//
//    template<typename Derived_>
//    struct NameComparer<Derived_*> {
//    public:
//        bool operator()(const Derived_* first, const Derived_* second) const {
//            return first->get_name() < second->get_name();
//        }
//    };

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

    template<typename D>
    struct OrderComparer<std::pair<uint32_t, D>> {
    public:
        bool operator()(
                const std::pair<uint32_t, D>& left,
                const std::pair<uint32_t, D>& right) const {
            return left.first < right.first;
        }
    };

    struct ActorDistanceComparer {
        const Actor *camera_ptr = nullptr;
        ActorDistanceComparer() = default;

        explicit ActorDistanceComparer(const Actor *camera) : camera_ptr(camera) {}

         _P_NODISCARD inline float get_distance(const glm::mat4 &mat_a) const {
            return glm::length<3, float>(glm::vec3(mat_a[3]) - glm::vec3(camera_ptr->transform[3]));
        }

        bool operator()(const Actor &act_a, const Actor &act_b) const {
            return get_distance(act_a.transform) < get_distance(act_b.transform);
        }

        bool operator()(const Actor * act_a, const Actor * act_b) const {
            return get_distance(act_a->transform) < get_distance(act_b->transform);
        }
    };
}

#endif //PONG_DATACOMPARERS_H
