//
// Created by Jordi on 6/12/2022.
//

#ifndef GL_TEST_PLANE_SHAPE_H
#define GL_TEST_PLANE_SHAPE_H

#include <iostream>
#include <string>
#include "../core/graphic_component.h"
#include "../core/edit_shape.h"
#include "../core/core_vals.h"

namespace Pong {

    struct PlaneSize{
        float width;
        float height;
    };

    class PlaneShape : public EditMesh {
    private:
        PlaneSize _sizes{1.f, 1.f};

        _P_NODISCARD _P_INLINE std::vector<glm::vec3> _computePlaneVertices() const {
            return {glm::vec3(_sizes.width / 2.f, _sizes.height / 2.f, 0.f),
                    glm::vec3(-_sizes.width / 2.f, _sizes.height / 2.f, 0.f),
                    glm::vec3(-_sizes.width / 2.f, -_sizes.height / 2.f, 0.f),
                    glm::vec3(_sizes.width / 2.f, -_sizes.height / 2.f, 0.f)};
        }

    public:
        explicit PlaneShape(Mesh *mesh, PlaneSize sizes) :
                EditMesh(mesh), _sizes(sizes) {
        }

    public:
        void build() override {
            EditMesh::build();
            std::vector<glm::vec3> tmpVert = _computePlaneVertices();

            for (unsigned int i = 0; i < 4; i++) {
                add_vertex(
                        {tmpVert[i],
                         {0.f, 0.f, 1.f},
                         (glm::vec2(0.5f, 0.5f) + (glm::vec2(tmpVert[i]) /
                                                   glm::vec2(_sizes.width, _sizes.height)))}
                );
            }

            // triangle index
            add_indices(0, 1, 3);
            add_indices(1, 2, 3);
        }
    };
}

#endif //GL_TEST_PLANE_SHAPE_H
