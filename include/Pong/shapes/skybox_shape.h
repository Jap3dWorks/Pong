//
// Created by Jordi on 6/13/2022.
//

#ifndef GL_TEST_SKYBOX_SHAPE_H
#define GL_TEST_SKYBOX_SHAPE_H
#include "Pong/core/shape.h"

namespace Pong {

    // Sky box Shape
    class SkyBoxShape : public Shape {
    private:
        void _build_sky_box() {
            _vertices.clear();
            _indices.clear();

            _vertices = {
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

            Shape::set_VAO();
        }

    public:
        void draw(const Render *render,
                  const Scene *scene,
                  Pong::Material *material) const override {
            glDepthFunc(GL_LEQUAL);

            if(!_indices.empty()){
                glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, vertex_count());
            }
            glDepthFunc(GL_LESS);
        }

        explicit SkyBoxShape(std::string name) :
                Shape(std::move(name)) {
            _build_sky_box();
        }
    };

}

#endif //GL_TEST_SKYBOX_SHAPE_H
