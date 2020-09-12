//
// Created by Jordi on 6/29/2020.
//

#ifndef PONG_LIGHTS_H
#define PONG_LIGHTS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pong {
    struct DirectionalLight {
        glm::vec4 direction = glm::vec4(0.f, -1.f, 0.f, 0.f);
        glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 0.f);
        glm::vec4 ambient = glm::vec4(0.f, 0.f, 0.f, 0.f);
    };

    struct PointLight
    {
        glm::vec4 position = glm::vec4(0.f, 0.f, 0.f, 0.f);
        glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 0.f);
        // float Linear;
        // float Quadratic;
        // float Radius;
        // float _PADDING;
    };
}

#endif //PONG_LIGHTS_H
