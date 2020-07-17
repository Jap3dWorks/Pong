//
// Created by Jordi on 7/17/2020.
//

#ifndef PONG_MAPKEYCOMPARER_H
#define PONG_MAPKEYCOMPARER_H

namespace Pong {
    class MapKeyComparer {

        template<typename T>
        bool operator()(const T &first, const T &second) {
            return first.order <= second.order;
        }

    };

}
#endif //PONG_MAPKEYCOMPARER_H
