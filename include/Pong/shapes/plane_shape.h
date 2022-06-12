//
// Created by Jordi on 6/12/2022.
//

#ifndef GL_TEST_PLANE_SHAPE_H
#define GL_TEST_PLANE_SHAPE_H

#include <iostream>
#include <string>
#include "Pong/core/shape.h"
#include "Pong/core/core_vals.h"

namespace Pong {
    class PlaneShape : public Shape {
    private:
        float _width;
        float _height;

        void _build_plane() {
            std::vector<glm::vec3> tmpVert = _computePlaneVertices();

            // clear prev arrays
            _vertices.clear();
            _indices.clear();

            for (unsigned int i = 0; i < 4; i++) {
                add_vertex(
                        {tmpVert[i],
                         {0.f, 0.f, 1.f},
                         (glm::vec2(0.5f, 0.5f) + (glm::vec2(tmpVert[i]) / glm::vec2(_width, _height)))}
                );
            }

            // triangle index
            add_indices(0, 1, 3);
            add_indices(1, 2, 3);
        }


        _P_NODISCARD _P_INLINE std::vector<glm::vec3> _computePlaneVertices() const {
            return {glm::vec3(_width / 2.f, _height / 2.f, 0.f),
                    glm::vec3(-_width / 2.f, _height / 2.f, 0.f),
                    glm::vec3(-_width / 2.f, -_height / 2.f, 0.f),
                    glm::vec3(_width / 2.f, -_height / 2.f, 0.f)};
        }

    public:
        explicit PlaneShape(const std::string &name, float height = 1.f, float width = 1.f) :
                _width(width), _height(height), Shape(name) {
            _build_plane();
            Shape::set_VAO();
        }
    };
}

#endif //GL_TEST_PLANE_SHAPE_H
