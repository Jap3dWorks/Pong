//
// Created by Jordi on 6/13/2022.
//

#ifndef GL_TEST_SKYBOX_SHAPE_H
#define GL_TEST_SKYBOX_SHAPE_H
#include "Pong/core/graphic_shape.h"
#include "Pong/core/edit_shape.h"

namespace Pong {
    class SkyBoxMesh : public EditMesh {
    private:
        void _build_sky_box() {
            _mesh->vertices.clear();
            _mesh->indices.clear();

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
        }

    public:
        explicit SkyBoxMesh() {
            _build_sky_box();
        }
    };

    class GraphicBoxMesh : GraphicMesh {
    public:
        void draw(const Render *render,
                  const Scene *scene,
                  Pong::Material *material) const override {
            glDepthFunc(GL_LEQUAL);

            if (!_mesh->indices.empty()) {
                glDrawElements(GL_TRIANGLES, _mesh->indices.size(),
                               GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, _mesh->vertices.size());
            }
            glDepthFunc(GL_LESS);
        }
    };

}

#endif //GL_TEST_SKYBOX_SHAPE_H
