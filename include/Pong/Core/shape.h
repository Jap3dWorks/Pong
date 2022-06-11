#ifndef SHAPE_H
#define SHAPE_H

#include "Pong/Core/utils.h"
#include "Pong/Core/primitive_component.h"
#include "Pong/Core/core_vals.h"

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
        uint32_t _VAO_id = 0;
        std::string _name;
        VertexVector _vertex;
        TriangleVector _indices;

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texture_coords;

        std::vector<uint32_t> indices;

        std::vector<uint32_t> line_indices;
        std::vector<float> interleaved_vertices;

        int interleavedStride = 32;  // (pos + normal + txtcoords) * 4

    public:
        uint32_t order{10};
        uint32_t draw_primitive = GL_TRIANGLES;

    private:
        inline void _add_vertices_(const float& x, const float& y, const float& z) {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }

        inline void _add_normals_(const float& x, const float& y, const float& z) {

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

        }

        inline void _add_tex_coords_(const float &u, const float &v) {
            texture_coords.push_back(u);
            texture_coords.push_back(v);
        }

        static inline void _compute_face_normal_(const glm::vec3& vtx0, const glm::vec3& vtx1,
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

    protected:
        // --geometry creation methods--
        // -----------------------------
        void build_interleaved_vertices();

        template<typename I, typename J, typename K>
        inline void add_indices(I&& i, J&& j, K&& k)
        {
            indices.push_back(std::forward<I>(i));
            indices.push_back(std::forward<J>(j));
            indices.push_back(std::forward<K>(k));
        }

        inline void add_tex_coords(const float &u, const float &v)
        {
            _add_tex_coords_(u, v);
        }
        inline void add_tex_coords(float &&u, float &&v)
        {
            _add_tex_coords_(u, v);
        }
        inline void add_tex_coords(float &&u, const float &v)
        {
            _add_tex_coords_(u, v);
        }

        inline void add_tex_coords(float * & uv)
        {
            _add_tex_coords_(uv[0], uv[1]);
        }

        template<typename T>
        inline void add_tex_coords(T&& t)
        {
            _add_tex_coords_(t[0], t[1]);
        }

        template<typename A, typename... Args>
        void add_tex_coords(A&& t, Args&&... args)
        {
            add_tex_coords(std::forward<A>(t));
            add_tex_coords(std::forward<Args>(args)...);
        }

        // add_vertices overloads
        // ----------------------
        inline void add_vertices(const float& x, const float& y, const float& z)
        {
            _add_vertices_(x, y, z);
        }
        inline void add_vertices(float&& x, float&& y, float&& z)
        {
            _add_vertices_(x, y, z);
        }

        inline void add_vertices(float * & vtx){
            _add_vertices_(vtx[0], vtx[1],  vtx[2]);
        }

        template<typename T>
        inline void add_vertices(T&& vtx)
        {
            _add_vertices_(vtx.x, vtx.y, vtx.z);
        }

        template<typename A, typename... Args>
        inline void add_vertices(A &&vtx, Args&&... args)
        {
            add_vertices(std::forward<A>(vtx));
            add_vertices(std::forward<Args>(args)...);
        }

        // add normals overloads
        // ---------------------
        inline void add_normals(const float& x, const float& y, const float& z)
        {
            _add_normals_(x, y, z);
        }
        inline void add_normals(float&& x, float&& y, float&& z)
        {
            _add_normals_(x, y, z);
        }

        inline void add_normals(float * & normal)
        {
            _add_normals_(normal[0], normal[1], normal[2]);
        }

        template<typename T>
        inline void add_normals(T &&normal) {
            _add_normals_(normal.x,
                          normal.y,
                          normal.z);
        }

        template<typename A, typename... Args>
        inline void add_normals(A&& normal, Args&&... args)
        {
            add_normals(std::forward<A>(normal));
            add_normals(std::forward<Args>(args)...);
        }

        template<typename T, typename U, typename V>
        static inline void compute_face_normal(T &&vtx0, U &&vtx1,
                                               V &&vtx2, glm::vec3 &out_normal) {
            Shape::_compute_face_normal_(
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
        explicit Shape(std::string name);
        virtual ~Shape() = default;

        _P_NODISCARD std::string get_name() const { return _name; }
        void set_name(std::string new_name) { _name = std::move(new_name);}

        _P_NODISCARD uint32_t vertex_count() const { return vertices.size() / 3; }
        _P_NODISCARD uint32_t normal_count() const { return (unsigned int)normals.size() / 3; }
        _P_NODISCARD uint32_t texture_coords_count() const { return (unsigned int)texture_coords.size() / 2; }
        _P_NODISCARD uint32_t get_index_count() const { return indices.size(); }
        _P_NODISCARD uint32_t get_line_index_count() const { return (unsigned int)line_indices.size(); }
        _P_NODISCARD uint32_t get_triangle_count() const { return get_index_count() / 3; }

        _P_NODISCARD uint32_t get_vertex_size() const { return (unsigned int)vertices.size() * sizeof(float); }
        _P_NODISCARD uint32_t get_normal_size() const { return (unsigned int)normals.size() * sizeof(float); }
        _P_NODISCARD uint32_t get_texture_coords_size() const {
            return (unsigned int)texture_coords.size() * sizeof(float); }
        _P_NODISCARD uint32_t get_index_size() const {
            return (unsigned int)indices.size() * sizeof(unsigned int); }
        _P_NODISCARD uint32_t get_line_index_size() const {
            return (unsigned int)line_indices.size() * sizeof(unsigned int); }

        _P_NODISCARD const float* get_vertices() const { return vertices.data(); }
        _P_NODISCARD const float* get_normals() const { return normals.data(); }
        _P_NODISCARD const float* get_texture_coords() const { return texture_coords.data(); }
        _P_NODISCARD const unsigned int* get_indices() const { return indices.data(); }
        _P_NODISCARD const unsigned int* get_line_indices() const { return line_indices.data(); }

        _P_NODISCARD const float * get_interleaved_vertices() const;
        _P_NODISCARD unsigned int get_interleaved_vertex_size() const;
        _P_NODISCARD unsigned int get_interleaved_vertex_count() const;

        _P_NODISCARD unsigned int get_VAO() const { return _VAO_id; }

        virtual void set_VAO() {
            int vRow = 0, nRow = 0, tRow = 0;
            if (vertex_count())
                vRow = 3;
            if (normal_count())
                nRow = 3;
            if (texture_coords_count())
                tRow = 2;

            interleavedStride = (vRow + nRow + tRow) * 4;

            unsigned int VBO, EBO;
            // reset vertex array
            glGenVertexArrays(1, &_VAO_id);
            glGenBuffers(1, &VBO);

            glBindVertexArray(_VAO_id);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER,
                         get_interleaved_vertex_size(),
                         interleaved_vertices.data(),
                         GL_STATIC_DRAW);

            // EBO object
            if (!indices.empty()) {
                glGenBuffers(1, &EBO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             get_index_size(),
                             indices.data(),
                             GL_STATIC_DRAW);
            }


            int rowSize = (vRow + nRow + tRow) * sizeof(float);
            int floatSize = sizeof(float);
            int offset = 0;
            int attrId = 0;

            // vertex positions attribute
            glEnableVertexAttribArray(attrId);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, interleavedStride,
                                  (void*)(offset * sizeof(float)));
            offset += vRow;
            attrId++;

            if (nRow) {
                // normal attribute
                glEnableVertexAttribArray(attrId);
                glVertexAttribPointer(attrId, nRow, GL_FLOAT, GL_FALSE, rowSize,
                                      (void*)(offset * sizeof(float)));
                offset += nRow;
                attrId++;
            }

            // uvs attribute
            if (tRow) {
                glEnableVertexAttribArray(attrId);
                glVertexAttribPointer(attrId, tRow, GL_FLOAT, GL_FALSE, rowSize,
                                      (void*)(offset * sizeof(float)));
            }
            // detach vertex array
            glBindVertexArray(0);
        }

        virtual void bind_VAO() {
            glBindVertexArray(_VAO_id);
        }


        virtual void draw(const Render *render, const Scene *scene, Pong::Material *material) const;
    };

    // Mesh
    class Mesh : public Shape{
    public:
        std::vector<Vertex> vertices;
        // constructor
        Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        void set_VAO() override;
    };


}

#endif // SHAPE_H
