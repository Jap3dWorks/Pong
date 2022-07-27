//
// Created by Jordi on 6/13/2022.
//

#ifndef GL_TEST_CUBEMAP_SHAPE_H
#define GL_TEST_CUBEMAP_SHAPE_H

#include "../core/graphic_component.h"
#include "../core/edit_shape.h"
#include "../core/primitive_component.h"

namespace Pong {

    class EditCubeMap : public EditMesh {
    public:
        explicit EditCubeMap(Mesh * _mesh):
        EditMesh(_mesh) {}

    public:
        void build() override {
            super::build();

            _mesh->vertices = {
                // positions
                {{-1.0f, 1.0f,  -1.0f}},
                {{-1.0f, -1.0f, -1.0f}},
                {{1.0f,  -1.0f, -1.0f}},
                {{1.0f,  -1.0f, -1.0f}},
                {{1.0f,  1.0f,  -1.0f}},
                {{-1.0f, 1.0f,  -1.0f}},

                {{-1.0f, -1.0f, 1.0f}},
                {{-1.0f, -1.0f, -1.0f}},
                {{-1.0f, 1.0f,  -1.0f}},
                {{-1.0f, 1.0f,  -1.0f}},
                {{-1.0f, 1.0f,  1.0f}},
                {{-1.0f, -1.0f, 1.0f}},

                {{1.0f,  -1.0f, -1.0f}},
                {{1.0f,  -1.0f, 1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{1.0f,  1.0f,  -1.0f}},
                {{1.0f,  -1.0f, -1.0f}},

                {{-1.0f, -1.0f, 1.0f}},
                {{-1.0f, 1.0f,  1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{1.0f,  -1.0f, 1.0f}},
                {{-1.0f, -1.0f, 1.0f}},

                {{-1.0f, 1.0f,  -1.0f}},
                {{1.0f,  1.0f,  -1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{1.0f,  1.0f,  1.0f}},
                {{-1.0f, 1.0f,  1.0f}},
                {{-1.0f, 1.0f,  -1.0f}},

                {{-1.0f, -1.0f, -1.0f}},
                {{-1.0f, -1.0f, 1.0f}},
                {{1.0f,  -1.0f, -1.0f}},
                {{1.0f,  -1.0f, -1.0f}},
                {{-1.0f, -1.0f, 1.0f}},
                {{1.0f,  -1.0f, 1.0f}}
            };

            // TODO: build indices
        }
    };

    class CubeMapMesh : MeshComponent {
    public:
        void by_frame(const Render *render,
                      const Map *scene,
                      Pong::Material *material) const override {
            glDepthFunc(GL_LEQUAL);

            if (!mesh->indices.empty()) {
                glDrawElements(GL_TRIANGLES, mesh->indices.size(),
                               GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
            }
            glDepthFunc(GL_LESS);
        }
    };

}

#endif // GL_TEST_CUBEMAP_SHAPE_H
