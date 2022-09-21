//
// Created by Jordi on 6/13/2022.
//

#ifndef GL_TEST_CUBEMAP_SHAPE_H
#define GL_TEST_CUBEMAP_SHAPE_H

#include "Pong/components/graphic_component.h"
#include "Pong/shape/edit_shape.h"
#include "Pong/core/geometry_data.h"

namespace pong {

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

            // TODO: build indices_val
        }
    };

    class CubeMapMesh : StaticMeshComp {
    public:
        void by_frame(const Render *render,
					  const Map *scene,
					  pong::Material *material) const override {
            glDepthFunc(GL_LEQUAL);

            if (!mesh->indices_val.empty()) {
                glDrawElements(GL_TRIANGLES, mesh->indices_val.size(),
                               GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
            }
            glDepthFunc(GL_LESS);
        }
    };

}

#endif // GL_TEST_CUBEMAP_SHAPE_H
