//
// Created by Jordi on 7/17/2020.
//

#ifndef PONG_MAPKEYCOMPARER_H
#define PONG_MAPKEYCOMPARER_H

namespace Pong {
    template<typename T>
    struct MapKeyComparer {
    public:
        bool operator()(const T& first, const T& second) const {
            return first.order <= second.order;
        }
    };

    template<typename T>
    struct MapKeyComparer<T*> {
    public:
        bool operator()(const T* first, const T* second) const {
            return first->order <= second->order;
        }
    };


}
#endif //PONG_MAPKEYCOMPARER_H
