//
// Created by Jordi on 6/13/2022.
//

#ifndef GL_TEST_SPHERE_SHAPE_H
#define GL_TEST_SPHERE_SHAPE_H

#include "Pong/core/graphic_component.h"
#include "Pong/core/edit_shape.h"
#include "Pong/core/primitive_component.h"
#include "Pong/logger.h"

namespace Pong {

    template<typename T>
    struct GlmCompare {
        bool operator()(const T &k1, const T &k2) const {
            return k1[0] < k2[0];
        }
    };

    using SharedMap = std::map<
            glm::vec2,
            uint32_t,
            GlmCompare<glm::vec2>
            >;

    class IcosphereShape : public EditMesh {
    public:
        explicit IcosphereShape(std::string name,
                                float radius = 1.f,
                                int subdivision = 2,
                                bool smooth = true) :
                _radius(radius),
                _subdivision(subdivision),
                _smooth(smooth),
                EditMesh(std::move(name)) {
            _build_icosphere();
        }

        ~IcosphereShape() override = default;

        void set_radius(float r) {
            _radius = r;
            _build_icosphere();
        }

    private:
        // --members--
        // -----------
        float _radius;
        int _subdivision;
        bool _smooth;
        std::vector<uint32_t> line_indices;


        const float S_STEP = 186 / 2048.f; // horizontal texture step
        const float T_STEP = 322 / 1024.f; // vertical texture step

        // map <uvCoords, index>
        SharedMap _shared_indices;

        // --methods--
        // -----------
        /**
            Build a smooth icosahedron, the basic icosahedron has 12 vertices, posterior subdivision
            are builded from this basic geometry, pushing new vertices using the radius length.*/
        void _build_vertices_smooth() {
            // compute de basic 12 vertices of a icosaedron
            // vertices with different texcoords cannot be shared.
            auto tmpVert = _prepare_icosphere_data();

            // smooth icosahedron has 14 non-shared vertices
            // and 8 shared vertices total 22 vertices.
            for (int i = 0; i < 5; i++)                        // v0, v1, v2, v3, v4 top
            {
                add_vertex({tmpVert[0],
                            {0.f, 0.f, 1.f},
                            {S_STEP + (S_STEP * (i * 2)), 0.f}});
            }

            for (int i = 0; i < 5; i++)                       // v5, v6, v7, v8, v9 bottom
            {
                add_vertex({tmpVert[11],
                            {0.f, 0.f, -1.f},
                            {S_STEP * 2 + (S_STEP * (i * 2)), T_STEP * 3}});
            }

            for (int i = 0; i < 2; i++) {
                // v10 right, v11 left
                add_vertex({tmpVert[1],
                            glm::normalize(tmpVert[1]),
                            {S_STEP * i * 10, T_STEP}});
            }

            for (int i = 0; i < 2; i++) {                     // v12 right, v13 left
                add_vertex({tmpVert[6],
                            glm::normalize(tmpVert[6]),
                            {S_STEP + (S_STEP * i * 10), T_STEP * 2}});
            }

            // 8 shared vertices
        for (int i = 0; i < 4; i++)                       // v14, v15, v16, v17 shared
        {
            add_vertex({tmpVert[i + 2],
                        glm::normalize(tmpVert[i + 2]),
                        {S_STEP * ((i + 1) * 2), T_STEP}});

//            _shared_indices[std::make_pair(S_STEP * ((i + 1) * 2), T_STEP)] = texture_coords.size() / 2 - 1;
        }

        for (int i = 0; i < 4; i++)
        {
            add_vertex({tmpVert[i + 7],                     // v18, v19, v20, v21 shared
                        glm::normalize(tmpVert[i + 7]),
                        {S_STEP * 3 + (S_STEP * i * 2), T_STEP * 2}});

//            _shared_indices[std::make_pair(S_STEP * 3 + (S_STEP * i * 2), T_STEP * 2)] = texture_coords.size() / 2 - 1;
        }

        // build triangles index list (20 triangles)
        add_indices(0, 10, 14);     // 1st row
        add_indices(1, 14, 15);
        add_indices(2, 15, 16);
        add_indices(3, 16, 17);
        add_indices(4, 17, 11);
        add_indices(10, 12, 14);    // 2nd row
        add_indices(12, 18, 14);
        add_indices(14, 18, 15);
        add_indices(18, 19, 15);
        add_indices(15, 19, 16);
        add_indices(19, 20, 16);
        add_indices(16, 20, 17);
        add_indices(20, 21, 17);
        add_indices(17, 21, 11);
        add_indices(21, 13, 11);
        add_indices(5, 18, 12);  // 3rd row
        add_indices(6, 19, 18);
        add_indices(7, 20, 19);
        add_indices(8, 21, 20);
        add_indices(9, 13, 21);

        // add edge lines of icosahedron
        line_indices.push_back(0);   line_indices.push_back(10); // 00 - 10
        line_indices.push_back(1);   line_indices.push_back(14); // 01 - 14
        line_indices.push_back(2);   line_indices.push_back(15); // 02 - 15
        line_indices.push_back(3);   line_indices.push_back(16); // 03 - 16
        line_indices.push_back(4);   line_indices.push_back(17); // 04 - 17
        line_indices.push_back(10);  line_indices.push_back(14); // 10 - 14
        line_indices.push_back(14);  line_indices.push_back(15); // 14 - 15
        line_indices.push_back(15);  line_indices.push_back(16); // 15 - 16
        line_indices.push_back(16);  line_indices.push_back(17); // 10 - 14
        line_indices.push_back(17);  line_indices.push_back(11); // 17 - 11
        line_indices.push_back(10);  line_indices.push_back(12); // 10 - 12
        line_indices.push_back(12);  line_indices.push_back(14); // 12 - 14
        line_indices.push_back(14);  line_indices.push_back(18); // 14 - 18
        line_indices.push_back(18);  line_indices.push_back(15); // 18 - 15
        line_indices.push_back(15);  line_indices.push_back(19); // 15 - 19
        line_indices.push_back(19);  line_indices.push_back(16); // 19 - 16
        line_indices.push_back(16);  line_indices.push_back(20); // 16 - 20
        line_indices.push_back(20);  line_indices.push_back(17); // 20 - 17
        line_indices.push_back(17);  line_indices.push_back(21); // 17 - 21
        line_indices.push_back(21);  line_indices.push_back(11); // 21 - 11
        line_indices.push_back(12);  line_indices.push_back(18); // 12 - 18
        line_indices.push_back(18);  line_indices.push_back(19); // 18 - 19
        line_indices.push_back(19);  line_indices.push_back(20); // 19 - 20
        line_indices.push_back(20);  line_indices.push_back(21); // 20 - 21
        line_indices.push_back(21);  line_indices.push_back(13); // 21 - 13
        line_indices.push_back(5);   line_indices.push_back(12); // 05 - 12
        line_indices.push_back(6);   line_indices.push_back(18); // 06 - 18
        line_indices.push_back(7);   line_indices.push_back(19); // 07 - 19
        line_indices.push_back(8);   line_indices.push_back(20); // 08 - 20
        line_indices.push_back(9);   line_indices.push_back(21); // 09 - 21

        // subdivide icosahedron
        _subdivide_vertices_smooth();
    }

        /**Build a faceted icosahedron, no shared vertices*/
        void _build_vertices_flat() {
            auto temp_vertex = _prepare_icosphere_data();

            Vertex v0{}, v1{}, v2{}, v3{}, v4{}, v11{};

//            glm::vec3 *v0, *v1, *v2, *v3, *v4, *v11;
            glm::vec3 normal;
//            glm::vec2 t0, t1, t2, t3, t4, t11;
            unsigned int index = 0;

            v0.position = temp_vertex[0];
            v11.position = temp_vertex[11];
            for(int i = 1; i<=5; ++i)
            {
                v1.position = temp_vertex[i];
                if(i<5){
                    v2.position = temp_vertex[i + 1];
                }
                else{
                    v2.position = temp_vertex[1];
                }

                v3.position = temp_vertex[i+5];
                if((i + 5) < 10){
                    v4.position = temp_vertex[i + 6];
                }
                else{
                    v4.position = temp_vertex[6];
                }

                // texture coords
                v0.tex_coords = {(2.f * i - 1.f) * S_STEP, 0};
                v1.tex_coords = {(2.f * i -2.f) * S_STEP,  T_STEP};
                v2.tex_coords = {(2.f * i - 0.f) * S_STEP, T_STEP};
                v3.tex_coords = {(2.f * i - 1.f) * S_STEP, T_STEP * 2};
                v4.tex_coords = {(2.f * i + 1.f) * S_STEP, T_STEP * 2};
                v11.tex_coords = {2.f * i * S_STEP,  T_STEP * 3};

                // triangles
                v0.normal = compute_face_normal(
                        v0.position, v1.position, v2.position);
                v1.normal = v0.normal;
                v2.normal = v0.normal;
                add_vertex(v0);
                add_vertex(v1);
                add_vertex(v2);
                add_indices(index, index + 1, index + 2);

                v1.normal = v3.normal = v2.normal =
                        compute_face_normal(v1.position, v3.position, v2.position);
                add_vertex(v1);
                add_vertex(v3);
                add_vertex(v2);
                add_indices(index + 3, index + 4, index + 5);

                v2.normal = v3.normal = v4.normal =
                        compute_face_normal(v2.position, v3.position, v4.position);
                add_vertex(v2);
                add_vertex(v3);
                add_vertex(v4);
                add_indices(index + 6, index + 7, index + 8);

                v3.normal = v11.normal = v4.normal =
                        compute_face_normal(v3.position, v11.position, v4.position);
                add_vertex(v3);
                add_vertex(v11);
                add_vertex(v4);
                add_indices(index + 9, index + 10, index + 11);

                // edge lines
                line_indices.push_back(index);  //
                line_indices.push_back(index + 1);
                line_indices.push_back(index + 3); //
                line_indices.push_back(index + 4);
                line_indices.push_back(index + 3); //
                line_indices.push_back(index + 5);
                line_indices.push_back(index + 4); //
                line_indices.push_back(index + 5);
                line_indices.push_back(index + 9); //
                line_indices.push_back(index + 10);
                line_indices.push_back(index + 9); //
                line_indices.push_back(index + 11);

                index += 12;
            }

            _subdivide_vertices_flat();
        }

        /**Helper function to init an icosahedron
          data in flat and smooth versions. Also clears prev arrays.*/
        inline std::vector<glm::vec3> _prepare_icosphere_data()
        {
            _vertices.clear();
            _indices.clear();

            return _computeIcosahedronVertices();
        }

        void _build_icosphere() {
            if (_smooth) _build_vertices_smooth();

            else _build_vertices_flat();

            // set vertex buffers
            GraphicComponent::set_VAO();
        }

        /**
        * Compute the 12 basic vertices of an icosahedron
        */
        _P_NODISCARD std::vector<glm::vec3> _computeIcosahedronVertices() const {
            const float PI = 3.1415926f;
            const float H_ANGLE = PI / 180 * 72;    // angle for each col
            const float V_ANGLE = atanf(1.f / 2);   // elevation angle

            std::vector<glm::vec3> vertices(12);    // 12 vertices
            int i1, i2;                             // _indices
            float z, xy;
            // each row starts form a diferent angle, radians
            float hAngle1 = -PI / 2 - H_ANGLE / 2;  // -126 deg
            float hAngle2 = -PI / 2;                // -90 deg

            // top vertex
            vertices[0] = glm::vec3(0, 0, _radius);

            // 5 verts per row
            for (int i = 1; i <= 5; i++) {
                i1 = i;
                i2 = (i + 5); // 5 verts offset 3th row

                z = _radius * sin(V_ANGLE);         // elevation
                xy = _radius * cos(V_ANGLE);

                vertices[i1] = glm::vec3(xy * cosf(hAngle1), xy * sinf(hAngle1), z);  // x
                vertices[i2] = glm::vec3(xy * cosf(hAngle2), xy * sinf(hAngle2), -z);

                // next horizontal angles
                hAngle1 += H_ANGLE;
                hAngle2 += H_ANGLE;
            }

            // bottom vertex
            vertices[11] = glm::vec3(0, 0, -_radius);

            return vertices;
        }

        /**
            subdivide the icosahedron vertices with smooth results*/
        void _subdivide_vertices_smooth() {
            std::vector<unsigned int> tmpIndices;
            int indexCount;

            unsigned int i1, i2, i3;    // _indices from original triangle
            const glm::vec3 *v1, *v2, *v3;  // ptr to original vertices of a triangle
            const glm::vec2 *t1, *t2, *t3;  // ptr to original texcorrds of a triangle

            uint32_t newI1, newI2, newI3;
            int32_t i, j;

            // iteration for subdivision
            for (i = 1; i <= _subdivision; ++i) {
                // copy prev _indices
                tmpIndices = _indices;

                // clear prev arrays
                _indices.clear();
                line_indices.clear();

                indexCount = (int) tmpIndices.size();
                for (j = 0; j < indexCount; j += 3) {
                    // get 3 _indices each triangle
                    i1 = tmpIndices[j];
                    i2 = tmpIndices[j + 1];
                    i3 = tmpIndices[j + 2];

                    // get 3 vertex attribs from prev triangle
                    v1 = &_vertices[i1 * 3].position;
                    v2 = &_vertices[i2 * 3].position;
                    v3 = &_vertices[i3 * 3].position;
                    t1 = &_vertices[i1 * 2].tex_coords;
                    t2 = &_vertices[i2 * 2].tex_coords;
                    t3 = &_vertices[i3 * 2].tex_coords;

                    // get 3 new vertex attribs by splitting half on each edge
                    auto newV1 = _compute_half_vertex(*v1, *v2, _radius);
                    auto newV2 = _compute_half_vertex(*v2, *v3, _radius);
                    auto newV3 = _compute_half_vertex(*v1, *v3, _radius);

                    auto newT1 = _compute_half_tex_coords(*t1, *t2);
                    auto newT2 = _compute_half_tex_coords(*t2, *t3);
                    auto newT3 = _compute_half_tex_coords(*t1, *t3);

                    auto newN1 = _compute_vertex_normal(newV1);
                    auto newN2 = _compute_vertex_normal(newV2);
                    auto newN3 = _compute_vertex_normal(newV3);

                    // add new vertices/normals/texcoords to arrays
                    // it will check if it is shared/non-shared and return index
                    newI1 = _add_subvertex_attribs({newV1, newN1, newT1});
                    newI2 = _add_subvertex_attribs({newV2, newN2, newT2});
                    newI3 = _add_subvertex_attribs({newV3, newN3, newT3});

                    // add 4 new triangle _indices
                    add_indices(i1, newI1, newI3);
                    add_indices(newI1, i2, newI2);
                    add_indices(newI1, newI2, newI3);
                    add_indices(newI3, newI2, i3);

                    // add new line _indices
                    _add_sub_line_indices(i1, newI1, i2, newI2, i3, newI3);
                }
            }
        }

        void _subdivide_vertices_flat() {

            decltype(_vertices) temp_vertices;
            decltype(_indices) temp_indices;

            int index_count;

            Vertex *v1, *v2, *v3;

            glm::vec3 normal;
            unsigned int index = 0;
            int i, j;

            for (i = 0; i <= _subdivision; ++i) {
                temp_vertices = _vertices;
                temp_indices = _indices;

                _vertices.clear();
                _indices.clear();
                line_indices.clear();

                index = 0;
                index_count = (int) temp_indices.size();
                for (j = 0; j < index_count; j += 3) {
                    v1 = &temp_vertices[temp_indices[j] * 3];
                    v2 = &temp_vertices[temp_indices[j + 1] * 3];
                    v3 = &temp_vertices[temp_indices[j + 2] * 3];

                    // get 3 new vertices
                    auto new_V1 = _compute_half_vertex(v1->position, v2->position, _radius);
                    auto new_V2 = _compute_half_vertex(v2->position, v3->position, _radius);
                    auto new_V3 = _compute_half_vertex(v1->position, v3->position, _radius);
                    auto new_T1 = _compute_half_tex_coords(v1->tex_coords, v2->tex_coords);
                    auto new_T2 = _compute_half_tex_coords(v2->tex_coords, v3->tex_coords);
                    auto new_T3 = _compute_half_tex_coords(v1->tex_coords, v3->tex_coords);

                    // add 4 new triangles
//                    add_vertex(v1)

                    auto normal = compute_face_normal(v1->position, new_V1, new_V3);
                    add_vertex({v1->position, normal, v1->tex_coords});
                    add_vertex({new_V1, normal, new_T1});
                    add_vertex({new_V3, normal, new_T3});

                    add_indices(index, index + 1, index + 2);

                    normal = compute_face_normal(new_V1, v2->position, new_V2);
                    add_vertex({new_V1, normal, new_T1});
                    add_vertex({v2->position, normal, v2->tex_coords});
                    add_vertex({new_V2, normal, new_T2});

                    add_indices(index + 3, index + 4, index + 5);

                    normal = compute_face_normal(new_V1, new_V2, new_V3);
                    add_vertex({new_V1, normal, new_T1});
                    add_vertex({new_V2, normal, new_T2});
                    add_vertex({new_V3, normal, new_T3});

                    add_indices(index + 6, index + 7, index + 8);

                    normal = compute_face_normal(new_V3, new_V2, v3->position);
                    add_vertex({new_V3, normal, new_T3});
                    add_vertex({new_V2, normal, new_V2});
                    add_vertex({v3->position, normal, v3->tex_coords});

                    add_indices(index + 9, index + 10, index + 11);

                    _add_sub_line_indices(index, index + 1, index + 4,
                                          index + 5, index + 11, index + 9);

                    index += 12;
                }
            }
        }

        inline glm::vec3 _compute_half_vertex(const glm::vec3 &v1, const glm::vec3 &v2, float length) {
            return glm::normalize(v1 + v2) * length;
        }

        static inline glm::vec2 _compute_half_tex_coords(const glm::vec2 &t1, const glm::vec2 &t2) {
            return (t1 + t2) / 2.f;
        }

        static inline glm::vec2 _compute_half_tex_coords(const glm::vec2 &t1,
                                                         const glm::vec2 &t2,
                                                         glm::vec2 &nt) {
            return (t1 + t2) / 2.f;
        }

        static glm::vec3 _compute_vertex_normal(const glm::vec3 &v) {
            return glm::normalize(v);
        }

        /**
            add vertex to arrays*/
        uint32_t _add_subvertex_attribs(const Vertex& vertex) {
            uint32_t index;
            // check if is shared vertex
            if (IcosphereShape::_is_shared_texture_coord(vertex.tex_coords)) {
                auto iter = _shared_indices.find(vertex.tex_coords);

                if (iter == _shared_indices.end()) {
                    add_vertex(vertex);
                    index = _vertices.size() / 2 - 1;
                    _shared_indices[vertex.tex_coords] = index;
                } else {
                    index = iter->second;
                }
            }
                // not shared index
            else {
                add_vertex(vertex);
                index = _vertices.size() / 2 - 1;
            }
            return index;
        }


        inline void _add_sub_line_indices(uint32_t i1,
                                          uint32_t i2,
                                          uint32_t i3,
                                          uint32_t i4,
                                          uint32_t i5,
                                          uint32_t i6) {
            line_indices.push_back(i1);      // i1 - i2
            line_indices.push_back(i2);
            line_indices.push_back(i2);      // i2 - i6
            line_indices.push_back(i6);
            line_indices.push_back(i2);      // i2 - i3
            line_indices.push_back(i3);
            line_indices.push_back(i2);      // i2 - i4
            line_indices.push_back(i4);
            line_indices.push_back(i6);      // i6 - i4
            line_indices.push_back(i4);
            line_indices.push_back(i3);      // i3 - i4
            line_indices.push_back(i4);
            line_indices.push_back(i4);      // i4 - i5
            line_indices.push_back(i5);
        }


        static inline bool _is_shared_texture_coord(const glm::vec2 &t) {
            // 20 non-shared line segments
            const float S = 1.f / 11;
            const float T = 1.f / 3;

           static glm::vec2 segments[] ={
                   {S, 0}, {0, T},                 // 00 - 05
                   {S, 0}, {S * 2, T},             // 00 - 06
                   {S * 3, 0}, {S * 2, T},         // 01 - 06
                   {S * 3, 0}, {S * 4, T},         // 01 - 07
                   {S * 5, 0}, {S * 4, T},         // 02 - 07
                   {S * 5, 0}, {S * 6, T},         // 02 - 08
                   {S * 7, 0}, {S * 6, T},         // 03 - 08
                   {S * 7, 0}, {S * 8, T},         // 03 - 09
                   {S * 9, 0}, {S * 8, T},         // 04 - 09
                   {S * 9, 0}, {1, T * 2},         // 04 - 14
                   {0, T}, {S * 2, 1},             // 05 - 15
                   {S * 3, T * 2}, {S * 2, 1},     // 10 - 15
                   {S * 3, T * 2}, {S * 4, 1},     // 10 - 16
                   {S * 5, T * 2}, {S * 4, 1},     // 11 - 16
                   {S * 5, T * 2}, {S * 6, 1},     // 11 - 17
                   {S * 7, T * 2}, {S * 6, 1},     // 12 - 17
                   {S * 7, T * 2}, {S * 8, 1},     // 12 - 18
                   {S * 9, T * 2}, {S * 8, 1},     // 13 - 18
                   {S * 9, T * 2}, {S * 10, 1},    // 13 - 19
                   {1, T * 2}, {S * 10, 1}};       // 14 - 19

            // check the point with all 20 line segments
            // if it is on the line segment, it is non-shared

            int count = (int) (sizeof(segments) / sizeof(segments[0]));
            for (int i = 0, j = 1; i < count; i += 2, j += 2) {
                if (GraphicComponent::is_on_line_segment(segments[i], segments[j], t))
                    return false;  // not shared
            }
            return true;
        }
    };
}

#endif //GL_TEST_SPHERE_SHAPE_H
