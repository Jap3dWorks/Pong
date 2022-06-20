#ifndef SHAPE_H
#define SHAPE_H

#include "Pong/core/utils.h"
#include "Pong/core/primitive_component.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/graphic_flags.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include <cstdarg>

namespace Pong{
    class Scene;
    class Render;
    class Material;
}

namespace Pong {
    using VertexVector = std::vector<Vertex>;
    using TriangleVector = std::vector<Triangle>;
    using IndexVector = std::vector<uint32_t>;

    // Exceptions
    class MeshException : public std::exception {
    public:
        explicit MeshException(char const* const error) noexcept :
                std::exception(error){}
    };

    //TODO: All _shapes class should use Vertex struct
    // TODO: separate model clas from controllers

    // _shapes classes
    class GraphicShape {

    protected:
        /**Vertex array buffer id*/
        // use Mesh*
        Mesh *_mesh = nullptr;
        uint32_t _vao_id = 0;

    public:
        uint32_t order{10};  // TODO: move to other place
        uint32_t draw_primitive = GL_TRIANGLES;

    public:
        GraphicShape() = default;

        _P_EXPLICIT GraphicShape(Mesh* mesh) :
                _mesh(mesh) {
        }

        virtual ~GraphicShape() = default;

        _P_NODISCARD unsigned int get_VAO() const { return _vao_id; }

        _P_NODISCARD _P_INLINE Mesh *get_mesh() const { return _mesh; }

        void _P_INLINE set_mesh(Mesh *mesh) {
            _mesh = mesh;
        }

        virtual void set_VAO() {
            // TODO: dynamic shapes
            uint32_t VBO, EBO;

            glGenVertexArrays(1,&_vao_id);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(_vao_id);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER,
                         _mesh->vertices.size() * sizeof(decltype(_mesh->vertices)::value_type),
                         _mesh->vertices.data(),
                         GL_STATIC_DRAW
                         );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         _mesh->indices.size() * sizeof(decltype(_mesh->indices)::value_type),
                         _mesh->indices.data(),
                         GL_STATIC_DRAW
                         );

            glEnableVertexAttribArray((uint32_t)VertexAttrId::POSITION);
            glVertexAttribPointer((uint32_t)VertexAttrId::POSITION,
                                  sizeof(Vertex::position) / sizeof(float),
                                  GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  (void*)offsetof(Vertex, position)
                                  );

            glEnableVertexAttribArray((uint32_t)VertexAttrId::NORMAL);
            glVertexAttribPointer((uint32_t)VertexAttrId::NORMAL,
                                  sizeof(Vertex::normal) / sizeof(float),
                                  GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  (void*)offsetof(Vertex, normal)
                                  );

            glEnableVertexAttribArray((uint32_t)VertexAttrId::TEX_COORDS);
            glVertexAttribPointer((uint32_t) VertexAttrId::TEX_COORDS,
                                  sizeof(Vertex::tex_coords) / sizeof(float),
                                  GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  (void *) offsetof(Vertex, tex_coords)
            );

            glBindVertexArray(0);

        }

        virtual void bind_VAO() {
            glBindVertexArray(_vao_id);
        }

        // _mesh draw in other class
        virtual void draw(const Render *render, const Scene *scene, Pong::Material *material) const {
            if (!_mesh->indices.empty()){
                glDrawElements(draw_primitive, _mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
            }
            else {
                glDrawArrays(draw_primitive, 0, _mesh->vertices.size());
            }
        }
    };

    // GraphicMesh
    class GraphicMesh : public GraphicShape {
    public:
        // constructor
        _P_EXPLICIT GraphicMesh(Mesh* mesh) :
                GraphicShape(mesh) {
            GraphicMesh::set_VAO();
        }
    };
}

#endif // SHAPE_H
