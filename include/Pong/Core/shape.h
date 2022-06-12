#ifndef SHAPE_H
#define SHAPE_H

#include "Pong/core/utils.h"
#include "Pong/core/primitive_component.h"
#include "Pong/core/core_vals.h"

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

    enum class VertexAttrId : uint32_t {
        POSITION=0,
        NORMAL=1,
        TEX_COORDS=2
    };

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

    // _shapes classes
    class Shape {

    protected:
        /**Vertex array buffer id*/
        uint32_t _VAO_id_ = 0;
        std::string _name;
        VertexVector _vertices;
        IndexVector _indices;

        std::vector<uint32_t> line_indices;
//        std::vector<float> interleaved_vertices;

    public:
        uint32_t order{10};
        uint32_t draw_primitive = GL_TRIANGLES;


    protected:
        // --geometry creation methods--
        // -----------------------------
        static inline void compute_face_normal(const glm::vec3& vtx0, const glm::vec3& vtx1,
                                               const glm::vec3& vtx2, glm::vec3& out_normal) {
            const float EPSILON = 0.000001f;

            out_normal[0] = out_normal[1] = out_normal[2] = 0.f;

            // edges v0-v1 v0-v2
            glm::vec3 edge1 = vtx1 - vtx0;
            glm::vec3 edge2 = vtx2 - vtx0;

            // cross product
            out_normal = glm::cross(edge1, edge2);

            glm::normalize(out_normal);
        }

        inline void add_vertex(const Vertex& vertex) {
            _vertices.push_back(vertex);
        }

        template<typename I, typename J, typename K>
        inline void add_indices(I&& i, J&& j, K&& k)
        {
            _indices.push_back(std::forward<I>(i));
            _indices.push_back(std::forward<J>(j));
            _indices.push_back(std::forward<K>(k));
        }


        template<typename T, typename U, typename V>
        static inline void compute_face_normal(T &&vtx0, U &&vtx1,
                                               V &&vtx2, glm::vec3 &out_normal) {
            Shape::compute_face_normal(
                    glm::vec3(vtx0[0], vtx0[1], vtx0[2]),
                    glm::vec3(vtx1[0], vtx1[1], vtx1[2]),
                    glm::vec3(vtx2[0], vtx2[1], vtx2[2]),
                    out_normal);
        }

        /**determine a point c is on the segment a-b*/
        static bool _is_on_line_segment(const float *a, const float *b, const float *c) {
            const float EPSILON = 0.0001f;

            // cross product must be 0 if c is on the line
            float cross = ((b[0] - a[0]) * (c[1] - a[1])) - ((b[1] - a[1]) * (c[0] - a[0]));
            if (cross > EPSILON || cross < -EPSILON)
                return false;

            // must be within a-b
            for (int i = 0; i < 2; i++)
            {
                if ((c[i] > a[i] && c[i] > b[i]) || (c[i] < a[i] && c[i] < b[i]))
                    return false;
            }
            return true;
        }

    public:
        _P_EXPLICIT Shape(const std::string &name): _name(name) {}

        _P_EXPLICIT Shape(std::string &&name): _name(std::move(name)) {}

        Shape(const std::string &name, const VertexVector &vertices) :
                _name(name), _vertices(vertices) {
        }

        Shape(std::string &&name, VertexVector &&vertices) :
                _name(std::move(name)), _vertices(std::move(vertices)) {
        }

        virtual ~Shape() = default;

        _P_NODISCARD std::string get_name() const { return _name; }
        void set_name(std::string new_name) { _name = std::move(new_name);}

        _P_NODISCARD uint32_t vertex_count() const { return _vertices.size(); }
        _P_NODISCARD uint32_t index_count() const { return _indices.size(); }
            
        _P_NODISCARD VertexVector& get_vertices() {
            return _vertices;
        }
        
        _P_NODISCARD IndexVector& get_indices() {
            return _indices;
        }
            
        _P_NODISCARD const unsigned int* get_line_indices() const { return line_indices.data(); }

        _P_NODISCARD unsigned int get_VAO() const { return _VAO_id_; }

        virtual void set_VAO() {
            // TODO: dynamic shapes
            uint32_t VBO, EBO;

            glGenVertexArrays(1,&_VAO_id_);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(_VAO_id_);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER,
                         _vertices.size() * sizeof(decltype(_vertices)::value_type),
                         _vertices.data(),
                         GL_STATIC_DRAW
                         );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         _indices.size() * sizeof(decltype(_indices)::value_type),
                         _indices.data(),
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
            glVertexAttribPointer((uint32_t)VertexAttrId::TEX_COORDS,
                                  sizeof(Vertex::tex_coords) / sizeof(float),
                                  GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex),
                                  (void*)offsetof(Vertex, tex_coords)
            );

            glBindVertexArray(0);

        }

        virtual void bind_VAO() {
            glBindVertexArray(_VAO_id_);
        }

        virtual void draw(const Render *render, const Scene *scene, Pong::Material *material) const {
            if(!_indices.empty()){
                glDrawElements(draw_primitive, _indices.size(), GL_UNSIGNED_INT, nullptr);
            }
            else {
                glDrawArrays(draw_primitive, 0, vertex_count());
            }
        }
    };

    // Mesh
    class Mesh : public Shape{
    public:
        // constructor
        Mesh(std::string name, VertexVector vertices, IndexVector indices):
            Shape(std::move(name), std::move(vertices)) {
            indices = std::move(indices);
            Mesh::set_VAO();
        }

        void set_VAO() override;

    };


}

#endif // SHAPE_H
