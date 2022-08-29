//
// Created by Jordi on 8/29/2022.
//

#ifndef PONG_SRC_PONG_COMPONENTS_MAP_COMPONENT_H_
#define PONG_SRC_PONG_COMPONENTS_MAP_COMPONENT_H_

#include "Pong/components/component.h"
#include <string>

namespace pong::component {

    struct PythonComponent: public Component {
        std::string script{};
    };

}
#endif //PONG_SRC_PONG_COMPONENTS_MAP_COMPONENT_H_
