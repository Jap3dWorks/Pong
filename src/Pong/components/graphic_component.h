#ifndef SHAPE_H
#define SHAPE_H

#include "Pong/core/geometry_data.h"
#include "Pong/components/component.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/graphic_flags.h"
#include "Pong/core/exceptions.h"
#include "Pong/core/map/map_reg.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <map>

#include <cstdarg>

//namespace Pong{
//    class Map;
//    class Render;
//    class Material;
//}

namespace Pong {
    class GraphicComponent: public Component {
    protected:
        using graphic_component_super = GraphicComponent;

    public:
//        inline void update() override {};
//        virtual void update(Actor* actor, Component* parent) = 0;
//        virtual void update() = 0;
//        virtual void bind() = 0;

        ~GraphicComponent() = default;
    };

    // StaticMeshComponent
    class StaticMeshComponent : public GraphicComponent {
    protected:
        /**Vertex array buffer id*/
        // files in disk
        uint32_t material{};
        RegId mesh{};

    public:
//        Mesh *mesh = nullptr;
//        Material *material=nullptr;

//        GraphicComponentType type=GraphicComponentType::STATIC;
//        GraphicPrimitives primitive = GraphicPrimitives::TRIANGLE;

    public:
        StaticMeshComponent() = default;
        ~StaticMeshComponent() = default;

//        _P_EXPLICIT StaticMeshComponent(Mesh* mesh) :
//                mesh(mesh) {
//        }

//        _P_NODISCARD uint32_t get_VAO() const { return _vao_id; }


        // TODO: RENDER functions in a render class
//        virtual void set_VAO() {
//            // TODO: dynamic shapes
//            uint32_t VBO, EBO;
//
//            glGenVertexArrays(1,&_vao_id);
//            glGenBuffers(1, &VBO);
//            glGenBuffers(1, &EBO);
//
//            glBindVertexArray(_vao_id);
//            glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//            glBufferData(GL_ARRAY_BUFFER,
//                         mesh->vertices.size() * sizeof(decltype(mesh->vertices)::value_type),
//                         mesh->vertices.data(),
//                         GL_STATIC_DRAW
//            );
//
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//                         mesh->indices.size() * sizeof(decltype(mesh->indices)::value_type),
//                         mesh->indices.data(),
//                         GL_STATIC_DRAW
//            );
//
//            glEnableVertexAttribArray((uint32_t)VertexAttrId::POSITION);
//            glVertexAttribPointer((uint32_t)VertexAttrId::POSITION,
//                                  sizeof(Vertex::position) / sizeof(float),
//                                  GL_FLOAT, GL_FALSE,
//                                  sizeof(Vertex),
//                                  (void*)offsetof(Vertex, position)
//            );
//
//            glEnableVertexAttribArray((uint32_t)VertexAttrId::NORMAL);
//            glVertexAttribPointer((uint32_t)VertexAttrId::NORMAL,
//                                  sizeof(Vertex::normal) / sizeof(float),
//                                  GL_FLOAT, GL_FALSE,
//                                  sizeof(Vertex),
//                                  (void*)offsetof(Vertex, normal)
//            );
//
//            glEnableVertexAttribArray((uint32_t)VertexAttrId::TEX_COORDS);
//            glVertexAttribPointer((uint32_t) VertexAttrId::TEX_COORDS,
//                                  sizeof(Vertex::tex_coords) / sizeof(float),
//                                  GL_FLOAT, GL_FALSE,
//                                  sizeof(Vertex),
//                                  (void *) offsetof(Vertex, tex_coords)
//            );
//
//            glBindVertexArray(0);
//
//        }

        // TODO: RENDER functions in a render class
//        void bind() override {
//            glBindVertexArray(_vao_id);
//        }

        // mesh update in other class
        // TODO: RENDER functions in a render class
//        void update(Actor* actor, Component* parent) override {
//            if (!mesh->indices.empty()){
//                glDrawElements(GL_TRIANGLES, mesh->indices.size(),
//                               GL_UNSIGNED_INT,
//                               nullptr);
//            }
//            else {
//                glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
//            }
//        }

    };
}

#endif // SHAPE_H
