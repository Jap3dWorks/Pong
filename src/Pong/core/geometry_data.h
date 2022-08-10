//
// Created by Jordi on 6/11/2022.
//

#ifndef GL_TEST_GEOMETRY_DATA_H
#define GL_TEST_GEOMETRY_DATA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <array>

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

    struct Ray {
        glm::vec3 point;
        glm::vec3 direction;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };


    enum class GraphicPrimitives: uint32_t {
        POINT=0,
        LINE=1,
        TRIANGLE=2,
        CAMERA=3
    };

    struct StackMesh_ {
    public:
        Vertex* const vertices{nullptr};
        uint32_t* const indices{nullptr};
    };

    template<size_t V, size_t I>
    struct StackMesh: public StackMesh_ {
    private:
        Vertex vertices_[V]{};
        uint32_t indices_[I]{};

    public:
        Vertex* const vertices{vertices_};
        uint32_t * const indices{indices_};

        uint32_t vertex_size{0};
        uint32_t indices_size{0};
    };
}

#endif //GL_TEST_GEOMETRY_DATA_H
