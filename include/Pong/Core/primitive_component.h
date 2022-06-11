//
// Created by Jordi on 6/11/2022.
//

#ifndef GL_TEST_PRIMITIVE_COMPONENT_H
#define GL_TEST_PRIMITIVE_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Pong {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
//        glm::vec3 tangent;
//        glm::vec3 bitangent;
    };

    struct Edge {
        uint32_t p0;
        uint32_t p1;
    };

    struct Triangle {
        uint32_t p0;
        uint32_t p1;
        uint32_t p2;
    };

}

#endif //GL_TEST_PRIMITIVE_COMPONENT_H
