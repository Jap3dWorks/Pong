//
// Created by Jordi on 7/17/2020.
//

#ifndef PONG_DATACOMPARERS_H
#define PONG_DATACOMPARERS_H

#include "logger.h"

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
        return first->order < second->order
    }
}
#endif //PONG_DATACOMPARERS_H
