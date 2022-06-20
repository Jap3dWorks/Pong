//
// Created by Jordi on 6/21/2022.
//

#ifndef GL_TEST_EDIT_SHAPE_H
#define GL_TEST_EDIT_SHAPE_H

#include <iostream>
#include "Pong/core/primitive_component.h"
#include "Pong/core/core_vals.h"

namespace Pong {
    class EditShape {
    };

    class EditMesh : public EditShape {
    protected:
        Mesh* _mesh=nullptr;

    public:
        EditMesh() = default;

        _P_EXPLICIT EditMesh(Mesh* mesh): _mesh(mesh) {}

        _P_STATIC glm::vec3 _P_INLINE compute_face_normal(const glm::vec3 &v1,
                                                          const glm::vec3 &v2,
                                                          const glm::vec3 &v3) {
            return glm::normalize(glm::cross(v2 - v1, v3 - v1));
        }

        inline void add_vertex(const Vertex& vertex) {
            _mesh->vertices.push_back(vertex);
        }

        template<typename I, typename J, typename K>
        inline void add_indices(I&& i, J&& j, K&& k) {
            _mesh->indices.push_back(std::forward<I>(i));
            _mesh->indices.push_back(std::forward<J>(j));
            _mesh->indices.push_back(std::forward<K>(k));
        }

        _P_NODISCARD _P_INLINE uint32_t vertex_count() const {
            return _mesh->vertices.size(); }

        _P_NODISCARD _P_INLINE uint32_t index_count() const {
            return _mesh->indices.size(); }

        /**determine a point c is on the segment a-b*/
        static bool is_on_line_segment(const glm::vec2 &a,
                                       const glm::vec2 &b,
                                       const glm::vec2 &c) {
            const float EPSILON = 0.0001f;

            // cross product must be 0 if c is on the line
//            float cross = glm::cross(a-c, b-c);
            float cross = ((b[0] - a[0]) * (c[1] - a[1])) - ((b[1] - a[1]) * (c[0] - a[0]));
            if (cross > EPSILON || cross < -EPSILON)
                return false;

            // must be within a-b
            for (int i = 0; i < 2; i++) {
                auto len = glm::length((a-c));
                auto line = glm::normalize(a - b);

                if (abs(glm::dot(a-c, line)) > len) {
                    return false;
                }
            }
            return true;
        }

        _P_NODISCARD _P_INLINE Mesh *get_mesh() const { return _mesh; }
        void _P_INLINE set_mesh(Mesh *mesh) {
            _mesh = mesh;
        }

    };
}

#endif //GL_TEST_EDIT_SHAPE_H
