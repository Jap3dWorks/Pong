#ifndef ACTOR_H
#define ACTOR_H

namespace Pong
{
    class Component;
    class Map;
    class Render;
    class Material;
}

#include "Pong/core/movements.h"
#include "Pong/core/collider.h"
#include "Pong/components/component.h"
#include "Pong/map/reg_data.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <list>

namespace Pong {
    class Actor {
    public:
        using AID = RegId;
        using const_AID = const RegId;
//        bool active = true;

    private:
        RegId reg_id_;

    public:
//        glm::mat4 transform = glm::mat4(1);

    private:
        explicit Actor(RegId reg_id) : reg_id_(reg_id) {}

    public:
        virtual ~Actor() = default;

        template<typename ...Types>
        friend class RegData;
    };
}

#endif // ACTOR_H
