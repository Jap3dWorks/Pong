//
// Created by Jordi on 6/11/2022.
//

#ifndef GL_TEST_SHAPE_PRIMITIVES_H
#define GL_TEST_SHAPE_PRIMITIVES_H

#include "Pong/Core/shape.h"
#include <iostream>
#include <string>


// Icosphere
// DOCUMENTATION: http://www.songho.ca/opengl/gl_sphere.html
namespace Pong {
    class IcosphereShape : public Shape {
    public:
        explicit IcosphereShape(std::string name, float radius = 1.f, int subdivision = 2, bool smooth = true);

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
        inline void _compute_half_vertex(const float *v1, const float *v2, float &length, T &nv) {
            glm::vec3 rVec = glm::normalize(
                    glm::vec3(v1[0], v1[1], v1[2]) + glm::vec3(v2[0], v2[1], v2[2])
            ) * length;

            nv[0] = rVec.x;
            nv[1] = rVec.y;
            nv[2] = rVec.z;
        }


        static inline void _compute_half_tex_coords(const float *t1, const float *t2, float *nt);

        static inline void _compute_half_tex_coords(const float *t1, const float *t2, glm::vec2 &nt);

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
    class CubeShape : public Shape {
    private:
        float _width;
        float _height;
        float _depth;

        void _buildCubeVerticesHard();

        [[nodiscard]] std::vector<glm::vec3> _computeCubeVertices() const;

    public:
        explicit CubeShape(std::string name, float height = 1.f, float width = 1.f, float depth = 1.f);

        ~CubeShape() override;
    };

    class PlaneShape : public Shape {
    private:
        float _width;
        float _height;

        void _build_plane();

        [[nodiscard]] std::vector<glm::vec3> _computePlaneVertices() const;

    public:
        explicit PlaneShape(const std::string &name, float height = 1.f, float width = 1.f);
    };

    // Sky box Shape
    class SkyBoxShape : public Shape {
    private:
        void _build_sky_box();

    public:
        void draw(const Render *render, const Scene *scene, Pong::Material *material) const override;
        explicit SkyBoxShape(std::string name);
    };
}


#endif //GL_TEST_SHAPE_PRIMITIVES_H
