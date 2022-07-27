#ifndef SHAPE_H
#define SHAPE_H

#include "primitive_component.h"
#include "component.h"
#include "core_vals.h"
#include "graphic_flags.h"

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
    class Map;
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

    class GraphicComponent: public Component {
        // mesh update in other class
    protected:
        using graphic_component_super = GraphicComponent;

    public:
//        void update(Actor* actor, Component* parent) override = 0;
        virtual void bind() = 0;

        ~GraphicComponent() = default;
    };

    // MeshComponent
    class MeshComponent : public GraphicComponent {
    protected:
//        using mesh_component_super = MeshComponent;

    protected:
        /**Vertex array buffer id*/
        // use Mesh*
        uint32_t _vao_id = 0;
        std::unique_ptr<Mesh> _internal_mesh;

    public:
        Mesh *mesh = nullptr;
        uint32_t draw_primitive = GL_TRIANGLES;

    public:
        MeshComponent() {
            _internal_mesh = std::make_unique<Mesh>();
            mesh = _internal_mesh.get();
        };

        _P_EXPLICIT MeshComponent(Mesh* mesh) :
                mesh(mesh) {
        }

        ~MeshComponent() = default;

        _P_NODISCARD uint32_t get_VAO() const { return _vao_id; }

        virtual void set_VAO() {
            // TODO: dynamic shapes
            uint32_t VBO, EBO;

            glGenVertexArrays(1,&_vao_id);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(_vao_id);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER,
                         mesh->vertices.size() * sizeof(decltype(mesh->vertices)::value_type),
                         mesh->vertices.data(),
                         GL_STATIC_DRAW
            );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         mesh->indices.size() * sizeof(decltype(mesh->indices)::value_type),
                         mesh->indices.data(),
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

        void bind() override {
            glBindVertexArray(_vao_id);
        }

        // mesh update in other class
        void update(Actor* actor, Component* parent) override {
            GraphicComponent::update(actor, parent);

            if (!mesh->indices.empty()){
                glDrawElements(draw_primitive, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
            }
            else {
                glDrawArrays(draw_primitive, 0, mesh->vertices.size());
            }
        }
    };
}

#endif // SHAPE_H
