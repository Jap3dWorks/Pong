//
// Created by Jordi on 6/12/2022.
//

#ifndef GL_TEST_CUBE_SHAPE_H
#define GL_TEST_CUBE_SHAPE_H

#include <iostream>
#include <string>
#include "Pong/core/shape.h"
#include "Pong/core/core_vals.h"

// TODO: cube subdivisions
namespace Pong {
    class CubeShape : public Shape {
    private:
        float _width;
        float _height;
        float _depth;

        void _buildCubeVerticesHard() {
            // compute de basic 8 vertices of a cube
            // vertices with different texcoords cannot be shared.
            std::vector<glm::vec3> tmpVert = _computeCubeVertices();

            // clear prev arrays
            _vertices.clear();
            _indices.clear();

            // face cube has 12 non-shared vertices
            // and 12 shared vertices total 24 vertices.
            glm::vec2 fUv[4] = {glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f),
                                glm::vec2(0.f, 1.f), glm::vec2(0.f, 0.f)};

            // face 1
            glm::vec3 fNr(0.f, 1.f, 0.f);
            add_vertex({tmpVert[0], fNr, fUv[0]});

            add_vertex({tmpVert[1], fNr, fUv[1]});                      // v1

            add_vertex({tmpVert[2], fNr, fUv[2]});                      // v2 shared

            add_vertex({tmpVert[3], fNr, fUv[3]});                      // v3

            //face 2
            fNr = glm::vec3(1.f, 0.f, 0.f);
            add_vertex({tmpVert[5], fNr, fUv[0]});                      // v4

            add_vertex({tmpVert[1], fNr, fUv[1]});                      // v5

            add_vertex({tmpVert[0], fNr, fUv[2]});                      // v6

            add_vertex({tmpVert[4], fNr, fUv[3]});                      // v7

            //face 3
            fNr = glm::vec3(0.f, 0.f, -1.f);
            add_vertex({tmpVert[6], fNr, fUv[0]});                     // v8

            add_vertex({tmpVert[2], fNr, fUv[1]});                     // v9

            add_vertex({tmpVert[1], fNr, fUv[2]});                     // v10

            add_vertex({tmpVert[5], fNr, fUv[3]});                     // v11

            //face 4
            fNr = glm::vec3(-1.f, 0.f, 0.f);
            add_vertex({tmpVert[7], fNr, fUv[0]});                     // v12

            add_vertex({tmpVert[3], fNr, fUv[1]});                     // v13

            add_vertex({tmpVert[2], fNr, fUv[2]});                     // v14

            add_vertex({tmpVert[6], fNr, fUv[3]});                     // v15

            //face 5
            fNr = glm::vec3(0.f, 0.f, 1.f);
            add_vertex({tmpVert[4], fNr, fUv[0]});                     // v16

            add_vertex({tmpVert[0], fNr, fUv[1]});                     // v17

            add_vertex({tmpVert[3], fNr, fUv[2]});                     // v18

            add_vertex({tmpVert[7], fNr, fUv[3]});                     // v19

            //face 6
            fNr = glm::vec3(0.f, -1.f, 0.f);
            add_vertex({tmpVert[4], fNr, fUv[0]});                     // v20

            add_vertex({tmpVert[5], fNr, fUv[1]});                     // v21

            add_vertex({tmpVert[6], fNr, fUv[2]});                     // v22

            add_vertex({tmpVert[7], fNr, fUv[3]});                     // v23

            // triangle index list 12 triangles
            add_indices(0, 1, 2);  // top face
            add_indices(0, 2, 3);
            add_indices(4, 5, 6);  // face 2
            add_indices(4, 6, 7);
            add_indices(8, 9, 10);  // face 3
            add_indices(8, 10, 11);
            add_indices(12, 13, 14);  // face 4
            add_indices(12, 14, 15);
            add_indices(16, 17, 18);  // face 5
            add_indices(16, 18, 19);
            add_indices(20, 22, 21);  // bottom face
            add_indices(20, 23, 22);
        }

        _P_NODISCARD std::vector<glm::vec3> _computeCubeVertices() const {
            std::vector<glm::vec3> tmpVert(8);

            // upper vertices
            tmpVert[0] = glm::vec3(_width / 2, _height / 2, _depth / 2);
            tmpVert[1] = glm::vec3(_width / 2, _height / 2, -_depth / 2);
            tmpVert[2] = glm::vec3(-_width / 2, _height / 2, -_depth / 2);
            tmpVert[3] = glm::vec3(-_width / 2, _height / 2, _depth / 2);

            // lower vertices
            tmpVert[4] = glm::vec3(_width / 2, -_height / 2, _depth / 2);
            tmpVert[5] = glm::vec3(_width / 2, -_height / 2, -_depth / 2);
            tmpVert[6] = glm::vec3(-_width / 2, -_height / 2, -_depth / 2);
            tmpVert[7] = glm::vec3(-_width / 2, -_height / 2, _depth / 2);

            return tmpVert;
        }

    public:
        explicit CubeShape(std::string name,
                           float height = 1.f,
                           float width = 1.f,
                           float depth = 1.f) :
                _width(width),
                _height(height),
                _depth(depth),
                Shape(std::move(name)) {
            _buildCubeVerticesHard();
            // set vertex buffers
            set_VAO();
        }

        ~CubeShape() override = default;
    };
}

#endif //GL_TEST_CUBE_SHAPE_H
