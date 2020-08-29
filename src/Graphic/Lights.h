//
// Created by Jordi on 6/29/2020.
//

#ifndef PONG_LIGHTS_H
#define PONG_LIGHTS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Pong {
    struct PointLight
    {
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);

    };
    struct DirectionalLight {
        glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
        glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 ambient = glm::vec3(0.f, 0.f, 0.f);
    };
}

#endif //PONG_LIGHTS_H
