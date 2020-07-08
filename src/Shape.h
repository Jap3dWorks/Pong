#ifndef SHAPE_H
#define SHAPE_H

#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include <cstdarg>

    // TODO: Spaceship primitive shape

namespace Pong {

    // Exceptions
    class MeshException : public std::exception {
    public:
        explicit MeshException(char const* const error) noexcept :
                std::exception(error){}
    };


    // data structures
    struct Vertex
    {
        //TODO: All _shapes class should use Vertex struct
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };


    // _shapes classes
    class Shape {
    public:
        explicit Shape(std::string name);
        virtual ~Shape() = default;

        [[nodiscard]] std::string get_name() const { return name; }
        void set_name(std::string new_name) {name = std::move(new_name);}

        [[nodiscard]] unsigned int get_vertex_count() const { return (unsigned int)vertices.size() / 3; }
        [[nodiscard]] unsigned int get_normal_count() const { return (unsigned int)normals.size() / 3; }
        [[nodiscard]] unsigned int get_texture_coords_count() const { return (unsigned int)texture_coords.size() / 2; }
        [[nodiscard]] unsigned int get_index_count() const { return indices.size(); }
        [[nodiscard]] unsigned int get_line_index_count() const { return (unsigned int)line_indices.size(); }
        [[nodiscard]] unsigned int get_triangle_count() const { return get_index_count() / 3; }

        [[nodiscard]] unsigned int get_vertex_size() const { return (unsigned int)vertices.size() * sizeof(float); }
        [[nodiscard]] unsigned int get_normal_size() const { return (unsigned int)normals.size() * sizeof(float); }
        [[nodiscard]] unsigned int get_texture_coords_size() const {
            return (unsigned int)texture_coords.size() * sizeof(float); }
        [[nodiscard]] unsigned int get_index_size() const {
            return (unsigned int)indices.size() * sizeof(unsigned int); }
        [[nodiscard]] unsigned int get_line_index_size() const {
            return (unsigned int)line_indices.size() * sizeof(unsigned int); }

        [[nodiscard]] const float* get_vertices() const { return vertices.data(); }
        [[nodiscard]] const float* get_normals() const { return normals.data(); }
        [[nodiscard]] const float* get_texture_coords() const { return texture_coords.data(); }
        [[nodiscard]] const unsigned int* get_indices() const { return indices.data(); }
        [[nodiscard]] const unsigned int* get_line_indices() const { return line_indices.data(); }

        [[nodiscard]] const float * get_interleaved_vertices() const;
        [[nodiscard]] unsigned int get_interleaved_vertex_size() const;
        [[nodiscard]] unsigned int get_interleaved_vertex_count() const;

        [[nodiscard]] unsigned int get_VAO() const { return VAO_id; }

        virtual void set_VAO();

        virtual void draw() const;

    private:
        inline void _add_vertices_(const float& x, const float& y, const float& z)
        {
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }

        inline void _add_normals_(const float& x, const float& y, const float& z) {

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

        }

        inline void _add_tex_coords_(const float &u, const float &v)
        {
            texture_coords.push_back(u);
            texture_coords.push_back(v);
        }

        static inline void _compute_face_normal_(const glm::vec3& vtx0, const glm::vec3& vtx1,
                                          const glm::vec3& vtx2, glm::vec3& out_normal);

    protected:
        /**Vertex array buffer id*/
        unsigned int VAO_id = 0;

        std::string name;
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> texture_coords;
        std::vector<unsigned int> indices;

        std::vector<unsigned int> line_indices;
        std::vector<float> interleaved_vertices;

        int interleavedStride = 32;  // (pos + normal + txtcoords) * 4

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
        inline void add_tex_coords(const float &u, float &&v)
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
        static bool _is_on_line_segment(const float *a, const float *b, const float *c);

    };

    // Icosphere
    // DOCUMENTATION: http://www.songho.ca/opengl/gl_sphere.html
    class IcosphereShape : public Shape
    {
    public:
        explicit IcosphereShape(std::string name, float radius=1.f, int subdivision=2, bool smooth=true);

        ~IcosphereShape() override;

        void set_radius(float r);

    private:
        // --members--
        // -----------
        float _radius;
        int _subdivision;
        bool _smooth;

        const float S_STEP = 186 / 2048.f; // horizontal texture step
        const float T_STEP = 322 / 1024.f; // vertical texture step

        // map <uvCoords, index>
        std::map<std::pair<float, float>, unsigned int> _shared_indices;

        // --methods--
        // -----------
        /**
            Build a smooth icosahedron, the basic icosahedron has 12 vertices, posterior subdivision
            are builded from this basic geometry, pushing new vertices using the radius length.*/
        void _build_vertices_smooth();

        /**Build a faceted icosahedron, no shared vertices*/
        void _build_vertices_flat();

        /**Helper function to init an icosahedron
          data in flat and smooth versions. Also clears prev arrays.*/
        inline std::vector<glm::vec3> _prepare_icosphere_data();

        void _build_icosphere();

        /**
            Compute the 12 basic vertices of an icosahedron*/
        std::vector<glm::vec3> _computeIcosahedronVertices() const;

        /**
            subdivide the icosahedron vertices with smooth results*/
        void _subdivide_vertices_smooth();
        void _subdivide_vertices_flat();

        template<typename T>
        inline void _compute_half_vertex(const float *v1, const float *v2, float& length, T& nv)
        {
            glm::vec3 rVec = glm::normalize(
                    glm::vec3(v1[0],v1[1],v1[2]) + glm::vec3(v2[0],v2[1],v2[2])
            ) * length;

            nv[0] = rVec.x;
            nv[1] = rVec.y;
            nv[2] = rVec.z;
        }


        static inline void _compute_half_tex_coords(const float *t1, const float *t2, float *nt);
        static inline void _compute_half_tex_coords(const float *t1, const float *t2, glm::vec2& nt);

        static void _compute_vertex_normal(const float *v, float *n);

        /**
            add vertex to arrays*/
        unsigned int _add_subvertex_attribs(const float *v, const float *n, const float *t);

        inline void _add_sub_line_indices(unsigned int i1,
                                          unsigned int i2,
                                          unsigned int i3,
                                          unsigned int i4,
                                          unsigned int i5,
                                          unsigned int i6);

        // --------------------
        // --static functions--
        // --------------------
        static bool _is_shared_texture_coord(const float *t);

    };

    // TODO: cube subdivisions
    class CubeShape : public Shape
    {
    private:
        float _width;
        float _height;
        float _depth;

        void _buildCubeVerticesHard();
        //void _buildCubeVertices();
        [[nodiscard]] std::vector<glm::vec3> _computeCubeVertices() const;

        std::map<std::pair<glm::vec3, glm::vec2>, unsigned int> _sharedIndices;

    public:
        explicit CubeShape(std::string name, float height=1.f, float width=1.f, float depth=1.f);

        ~CubeShape() override;
    };

    // Mesh
    class Mesh : public Shape{
    public:
        std::vector<Vertex> vertices;

        // constructor
        Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        void set_VAO() override;

    };

    // Sky box shape
    class SkyBoxShape : public Shape {
    private:
        void _build_sky_box(); // TODO, rev draw requirements.

    public:
        explicit SkyBoxShape(std::string name);
    };
}


#endif // SHAPE_H



