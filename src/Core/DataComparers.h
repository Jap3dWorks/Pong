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
        const ACamera *camera = nullptr;

        explicit ActorCameraDistanceComparer(const ACamera *_camera) : camera(_camera) {}

        static inline float get_distance(const glm::mat4 &mata, const glm::mat4 &matb) {
            return glm::length(glm::vec3(mata[3]) - glm::vec3(matb[3]));
        }

        bool operator()(const Actor &act_a, const Actor &act_b) const {
            return get_distance(act_a.get_transform(), camera->get_transform()) <
                   get_distance(act_b.get_transform(), camera->get_transform());
        }

        bool operator()(Actor* act_a, Actor* act_b) const {
            return get_distance(act_a->get_transform(), camera->get_transform()) <
                   get_distance(act_b->get_transform(), camera->get_transform());
        }

    };
}
#endif //PONG_DATACOMPARERS_H
