#ifndef SHAPE_H
#define SHAPE_H

#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <map>

#include <cstdarg>

namespace Pong {
    class Shape {
    public:
        Shape(std::string name);
        virtual ~Shape() {}

        [[nodiscard]] std::string getName() const { return _name; }

        [[nodiscard]] unsigned int getVertexCount() const { return(unsigned int)_vertices.size() / 3; }
        [[nodiscard]] unsigned int getNormalCount() const { return (unsigned int)_normals.size() / 3; }
        [[nodiscard]] unsigned int getTexCoordsCount() const { return (unsigned int)_tex_coords.size() / 2; }
        [[nodiscard]] unsigned int getIndexCount() const { return _indices.size(); }
        [[nodiscard]] unsigned int getLineIndexCount() const { return (unsigned int)_lineIndices.size(); }
        [[nodiscard]] unsigned int getTriangleCount() const { return getIndexCount() / 3; }

        [[nodiscard]] unsigned int getVertexSize() const { return (unsigned int)_vertices.size() * sizeof(float); }
        [[nodiscard]] unsigned int getNormalSize() const { return (unsigned int)_normals.size() * sizeof(float); }
        [[nodiscard]] unsigned int getTexCoordsSize() const { return (unsigned int)_tex_coords.size() * sizeof(float); }
        [[nodiscard]] unsigned int getIndexSize() const { return (unsigned int)_indices.size() * sizeof(unsigned int); }
        [[nodiscard]] unsigned int getLineIndexSize() const { return (unsigned int)_lineIndices.size() * sizeof(unsigned int); }

        [[nodiscard]] const float* getVertices() const { return _vertices.data(); }
        [[nodiscard]] const float* getNormals() const { return _normals.data(); }
        [[nodiscard]] const float* getTexCoords() const { return _tex_coords.data(); }
        [[nodiscard]] const unsigned int* getIndices() const { return _indices.data(); }
        [[nodiscard]] const unsigned int* getLineIndices() const { return _lineIndices.data(); }

        [[nodiscard]] const float * getInterleavedVertices() const;
        [[nodiscard]] unsigned int getInterleavedVertexSize() const;
        [[nodiscard]] unsigned int getInterleavedVertexCount() const;

        [[nodiscard]] unsigned int getVAO() const { return VAO_id; }

        void setVAO();

        virtual void draw() const;

    private:
        inline void _add_vertices_(const float& x, const float& y, const float& z)
        {
            _vertices.push_back(x);
            _vertices.push_back(y);
            _vertices.push_back(z);
        }

        inline void _add_normals_(const float& x, const float& y, const float& z) {

            _normals.push_back(x);
            _normals.push_back(y);
            _normals.push_back(z);

        }

        inline void _add_tex_coords_(const float &u, const float &v)
        {
            _tex_coords.push_back(u);
            _tex_coords.push_back(v);
        }

    protected:
        /**Vertex array buffer id*/
        unsigned int VAO_id = 0;

        std::string _name;
        std::vector<float> _vertices;
        std::vector<float> _normals;
        std::vector<float> _tex_coords;
        std::vector<unsigned int> _indices;
        std::vector<unsigned int> _lineIndices;
        std::vector<float> _interleavedVertices;

        int _interleavedStride = 32;  // (pos + normal + txtcoords) * 4

        // --geomtry creation methods--

        void _buildInterleavedVertices();

        template<typename I, typename J, typename K>
        inline void _add_indices(I&& i, J&& j, K&& k)
        {
            _indices.push_back(std::forward<T>(i));
            _indices.push_back(std::forward<T>(j));
            _indices.push_back(std::forward<T>(k));
        }

        inline void add_tex_coords(float &u, float &v)
        {
            _add_tex_coords_(u, v);
        }
        inline void add_tex_coords(const float &u, const float &v)
        {
            _add_tex_coords_(u, v);
        }
        inline void add_tex_coords(float &&u, float &&v)
        {
            _add_tex_coords_(u, v);
        }
        inline void add_tex_coords(float * & uv)
        {
            _add_tex_coords_(uv[0], uv[1]);
        }
//        inline void add_tex_coords(){}

        template<typename T>
        inline void add_tex_coords(T&& t)
        {
            add_tex_coords(t.x, t.y);
        }

        template<typename A, typename... Args>
        void add_tex_coords(A&& t, Args&... args)
        {
            add_tex_coords(std::forward<A>(t));
            add_tex_coords(args...);
        }

        // add_vertices overloads
        // ----------------------
        inline void add_vertices(const float& x, const float& y, const float& z)
        {
            _add_vertices_(x, y, z);
        }
//        inline void add_vertices(float& x, float& y, float& z)
//        {
//            _add_vertices_(x, y, z);
//        }
//        inline void add_vertices(float&& x, float&& y, float&& z)
//        {
//            _add_vertices_(x, y, z);
//        }

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
        inline void add_normals(float& x, float& y, float& z)
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
        inline void add_normals(A&& normal, Args&... args)
        {
            add_normals(std::forward<A>(normal));
            add_normals(args...);
        }

        /**Computes de cross product as the normal of the three given vertices*/
        static inline void compute_face_normal(const glm::vec3& vtx0, const glm::vec3& vtx1,
                                 const glm::vec3& vtx2, glm::vec3& out_normal);

        static inline void compute_face_normal(float* & vtx0, float * & vtx1,
                                        float* & vtx2, glm::vec3& out_normal);

        static inline void compute_face_normal(float* & vtx0, float glm::vec3& vtx1)

    };

    // Icosphere
    // DOCUMENTATION: http://www.songho.ca/opengl/gl_sphere.html
    class IcosphereShape : public Shape
    {
    public:
        // TODO: implemented facetaded Icosphere
        explicit IcosphereShape(std::string name, float radius=1.f, int subdivision=2, bool smooth=true);

        ~IcosphereShape() override;

        void setRadius(float r);

    private:
        // --members--
        float _radius;
        int _subdivision;
        bool _smooth;

        const float S_STEP = 186 / 2048.f; // horizontal texture step
        const float T_STEP = 322 / 1024.f; // vertical texture step

        // map <uvCoords, index>
        std::map<std::pair<float, float>, unsigned int> _shared_indices;

        // -----------
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

        void buildIcosphere();

        /**
            Compute the 12 basic vertices of an icosahedron*/
        std::vector<glm::vec3> _computeIcosahedronVertices();

        /**
            subdivide the icosahedron vertices with smooth results*/
        void _subdivide_vertices_smooth();
        void _subdivide_vertices_flat();

        inline void compute_half_vertex(const float *v1, const float *v2, float length, float *nv);
        inline void compute_half_vertex(const float v1[3], const float v2[3], float& length, glm::vec3& nv);

        inline void compute_half_tex_coords(const float *t1, const float *t2, float *nt);
        inline void compute_half_tex_coords(const float *t1, const float *t2, glm::vec2& nt);

        void _computeVertexNormal(const float v[3], float n[3]);
        /**
            add vertex to arrays
        */
        unsigned int _addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

        inline void _add_sub_line_indices(unsigned int i1,
                                          unsigned int i2,
                                          unsigned int i3,
                                          unsigned int i4,
                                          unsigned int i5,
                                          unsigned int i6);

        // --------------------
        // --static functions--
        // --------------------
        static bool _isSharedTexCoord(const float t[2]);

        /**
            determine a point c is on the segment a-b
        */
        static bool _isOnLineSegment(const float a[2], const float b[2], const float c[2]);
    };

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


}
#endif // SHAPE_H



