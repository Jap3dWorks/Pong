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
        [[nodiscard]] unsigned int getTexCoordsCount() const { return (unsigned int)_texCoords.size() / 2; }
        [[nodiscard]] unsigned int getIndexCount() const { return _indices.size(); }
        [[nodiscard]] unsigned int getLineIndexCount() const { return (unsigned int)_lineIndices.size(); }
        [[nodiscard]] unsigned int getTriangleCount() const { return getIndexCount() / 3; }

        [[nodiscard]] unsigned int getVertexSize() const { return (unsigned int)_vertices.size() * sizeof(float); }
        [[nodiscard]] unsigned int getNormalSize() const { return (unsigned int)_normals.size() * sizeof(float); }
        [[nodiscard]] unsigned int getTexCoordsSize() const { return (unsigned int)_texCoords.size() * sizeof(float); }
        [[nodiscard]] unsigned int getIndexSize() const { return (unsigned int)_indices.size() * sizeof(unsigned int); }
        [[nodiscard]] unsigned int getLineIndexSize() const { return (unsigned int)_lineIndices.size() * sizeof(unsigned int); }

        [[nodiscard]] const float* getVertices() const { return _vertices.data(); }
        [[nodiscard]] const float* getNormals() const { return _normals.data(); }
        [[nodiscard]] const float* getTexCoords() const { return _texCoords.data(); }
        [[nodiscard]] const unsigned int* getIndices() const { return _indices.data(); }
        [[nodiscard]] const unsigned int* getLineIndices() const { return _lineIndices.data(); }

        [[nodiscard]] const float * getInterleavedVertices() const;
        [[nodiscard]] unsigned int getInterleavedVertexSize() const;
        [[nodiscard]] unsigned int getInterleavedVertexCount() const;

        [[nodiscard]] unsigned int getVAO() const { return VAO_id; }

        void setVAO();

        virtual void draw() const;

    protected:
        /**Vertex array buffer id*/
        unsigned int VAO_id = 0;

        std::string _name;
        std::vector<float> _vertices;
        std::vector<float> _normals;
        std::vector<float> _texCoords;
        std::vector<unsigned int> _indices;
        std::vector<unsigned int> _lineIndices;
        std::vector<float> _interleavedVertices;

        int _interleavedStride = 32;  // (pos + normal + txtcoords) * 4

        // --methods--
        void _addIndices(unsigned int i, unsigned int j, unsigned int k)
        { _indices.push_back(i); _indices.push_back(j); _indices.push_back(k); }
        void _buildInterleavedVertices();

        inline void _add_tex_coords(float u, float v)
        {
            _texCoords.push_back(u);
            _texCoords.push_back(v);
        }

        // add tex coords
        inline void _add_tex_coords(const float t[2])  // array by reference
        {
            _texCoords.push_back(t[0]);
            _texCoords.push_back(t[1]);
        }

        inline void _add_tex_coords(){}

        template<typename T>
        inline void _add_tex_coords(const T&& t)
        {
            _texCoords.push_back(t.x);
            _texCoords.push_back(t.y);
        }

        template<typename A, typename... T>
        void _add_tex_coords(A&& t, T&... args)
        {
            _add_tex_coords(std::forward<A>(t));
            _add_tex_coords(args...);
        }

        inline void _add_vertices(const float& x, const float& y, const float& z)
        {
            _vertices.push_back(x);
            _vertices.push_back(y);
            _vertices.push_back(z);
        }
        inline void _add_vertices(const glm::vec3& vtx)
        {
            _vertices.push_back(vtx.x);
            _vertices.push_back(vtx.y);
            _vertices.push_back(vtx.z);
        }

        template<typename... T>
        inline void _add_vertices(const glm::vec3& vtx, T&... args)
        {
            _add_vertices(vtx);
            _add_vertices(args...);
        }

        // TODO: errors here
        // add normals functions  // TODO: write templates passing references
        inline void _add_normals(const float& x, const float& y, const float& z)
        {
            _normals.push_back(x);
            _normals.push_back(y);
            _normals.push_back(z);
        }
        inline void _add_normals(const glm::vec3& normal)
        {
            _normals.push_back(normal.x);
            _normals.push_back(normal.y);
            _normals.push_back(normal.z);
        }

        template<typename... T>
        inline void _add_normals(const glm::vec3& normal, T&... args)
        {
            _add_normals(normal);
            _add_normals(args...);
        }

        /**Computes de cross product as the normal of the three given vertices*/
        static void compute_face_normal(const glm::vec3& vtx0, const glm::vec3& vtx1,
                                 const glm::vec3& vtx2, glm::vec3& out_normal);
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
        std::map<std::pair<float, float>, unsigned int> _sharedIndices;

        // -----------
        // --methods--
        // -----------
        /**
            Build a smooth icosahedron, the basic icosahedron has 12 vertices, posterior subdivision
            are builded from this basic geometry, pushing new vertices using the radius length.*/
        void _buildVerticesSmooth();

        /**Helper function to init an icosahedron
          data in flat and smooth versions. Also clears prev arrays.*/
        inline std::vector<glm::vec3> _prepare_icosphere_data();

        void buildIcosphere();

        /**Build a faceted icosahedron, no shared vertices*/
         void _buildVerticesFlat();

        /**
            Compute the 12 basic vertices of an icosahedron
        */
        std::vector<glm::vec3> _computeIcosahedronVertices();

        /**
            subdivide the icosahedron vertices with smooth results
        */
        void _subdivideVerticesSmooth();

        void computeHalfVertex(const float v1[3], const float v2[3], float length, float nv[3]);
        void _computeHalfTexCoord(const float t1[2], const float t2[2], float nt[2]);
        void _computeVertexNormal(const float v[3], float n[3]);
        /**
            add vertex to arrays
        */
        unsigned int _addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

        void _addSubLineIndices(unsigned int i1,
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



