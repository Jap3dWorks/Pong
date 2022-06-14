#include "Pong/core/shape.h"
#include "Pong/shapes/shape_primitives.h"
#include "Pong/logger.h"
//#include "plane_shape.h"
#include <assimp/Importer.hpp>

namespace Pong {

    // --Shape--
    // ---------
//    void Shape::build_interleaved_vertices()
//    {
//        interleaved_vertices.clear();
//
//        std::size_t i, j;
//        std::size_t count = vertices.size();
//        for (i = 0, j = 0; i < count; i += 3, j += 2)
//        {
//            interleaved_vertices.push_back(vertices[i]);
//            interleaved_vertices.push_back(vertices[i + 1]);
//            interleaved_vertices.push_back(vertices[i + 2]);
//
//            interleaved_vertices.push_back(normals[i]);
//            interleaved_vertices.push_back(normals[i + 1]);
//            interleaved_vertices.push_back(normals[i + 2]);
//
//            interleaved_vertices.push_back(texture_coords[j]);
//            interleaved_vertices.push_back(texture_coords[j + 1]);
//        }
//    }


    // --Icosphere--
    // -------------

//    inline std::vector<glm::vec3> IcosphereShape::_prepare_icosphere_data()
//    {
//        vertices.clear();
//        normals.clear();
//        texture_coords.clear();
//        _indices.clear();
//
//        return _computeIcosahedronVertices();
//    }
//
//    void IcosphereShape::_build_vertices_flat()
//    {
//        auto temp_vertex = _prepare_icosphere_data();
//
//        glm::vec3 *v0, *v1, *v2, *v3, *v4, *v11;
//        glm::vec3 normal;
//        glm::vec2 t0, t1, t2, t3, t4, t11;
//        unsigned int index = 0;
//
//        v0 = &temp_vertex[0];
//        v11 = &temp_vertex[11];
//        for(int i = 1; i<=5; ++i)
//        {
//            v1 = &temp_vertex[i];
//            if(i<5){
//                v2 = &temp_vertex[i + 1];
//            }
//            else{
//                v2 = &temp_vertex[1];
//            }
//
//            v3 = &temp_vertex[i+5];
//            if((i + 5) < 10){
//                v4 = &temp_vertex[i + 6];
//            }
//            else{
//                v4 = &temp_vertex[6];
//            }
//
//            // texture coords
//            t0[0] = (2 * i - 1) * S_STEP; t0[1] = 0;
//            t1[0] = (2 * i -2) * S_STEP;  t1[1] = T_STEP;
//            t2[0] = (2 * i - 0) * S_STEP; t2[1] = T_STEP;
//            t3[0] = (2 * i - 1) * S_STEP; t3[1] = T_STEP * 2;
//            t4[0] = (2 * i + 1) * S_STEP; t4[1] = T_STEP * 2;
//            t11[0] = 2 * i * S_STEP; t11[1] = T_STEP * 3;
//
//            // triangles
//            IcosphereShape::compute_face_normal(*v0, *v1, *v2, normal);
//            add_vertices(*v0, *v1, *v2);
//            add_normals(normal, normal, normal);
//            add_tex_coords(t0, t1, t2);
//            add_indices(index, index + 1, index + 2);
//
//            IcosphereShape::compute_face_normal(*v1, *v3, *v2, normal);
//            add_vertices(*v1, *v3, *v2);
//            add_normals(normal, normal, normal);
//            add_tex_coords(t1, t3, t2);
//            add_indices(index + 3, index + 4, index + 5);
//
//            IcosphereShape::compute_face_normal(*v2, *v3, *v4, normal);
//            add_vertices(*v2, *v3, *v4);
//            add_normals(normal, normal, normal);
//            add_tex_coords(t1, t3, t4);
//            add_indices(index + 6, index + 7, index + 8);
//
//            IcosphereShape::compute_face_normal(*v3, *v11, *v4, normal);
//            add_vertices(*v3, *v11, *v4);
//            add_normals(normal, normal, normal);
//            add_tex_coords(t3, t11, t4);
//            add_indices(index + 9, index + 10, index + 11);
//
//            // edge lines
//            line_indices.push_back(index);  //
//            line_indices.push_back(index + 1);
//            line_indices.push_back(index + 3); //
//            line_indices.push_back(index + 4);
//            line_indices.push_back(index + 3); //
//            line_indices.push_back(index + 5);
//            line_indices.push_back(index + 4); //
//            line_indices.push_back(index + 5);
//            line_indices.push_back(index + 9); //
//            line_indices.push_back(index + 10);
//            line_indices.push_back(index + 9); //
//            line_indices.push_back(index + 11);
//
//            index += 12;
//        }
//
//        _subdivide_vertices_flat();
//
//        build_interleaved_vertices();
//    }

//    void IcosphereShape::_subdivide_vertices_smooth()
//    {
//        std::vector<unsigned int> tmpIndices;
//        int indexCount;
//        unsigned int i1, i2, i3;    // _indices from original triangle
//        const float *v1, *v2, *v3;  // ptr to original vertices of a triangle
//        const float *t1, *t2, *t3;  // ptr to original texcorrds of a triangle
//        float newV1[3], newV2[3], newV3[3];
//        float newN1[3], newN2[3], newN3[3];
//        float newT1[3], newT2[3], newT3[3];
//        unsigned int newI1, newI2, newI3;
//        int i, j;
//
//        // iteration for subdivision
//        for (i = 1; i <= _subdivision; ++i)
//        {
//            // copy prev _indices
//            tmpIndices = _indices;
//
//            // clear prev arrays
//            _indices.clear();
//            line_indices.clear();
//
//            indexCount = (int)tmpIndices.size();
//            for (j = 0; j < indexCount; j += 3)
//            {
//                // get 3 _indices each triangle
//                i1 = tmpIndices[j];
//                i2 = tmpIndices[j + 1];
//                i3 = tmpIndices[j + 2];
//
//                // get 3 vertex attribs from prev triangle
//                v1 = &vertices[i1 * 3];
//                v2 = &vertices[i2 * 3];
//                v3 = &vertices[i3 * 3];
//                t1 = &texture_coords[i1 * 2];
//                t2 = &texture_coords[i2 * 2];
//                t3 = &texture_coords[i3 * 2];
//
//                // get 3 new vertex attribs by splitting half on each edge
//                _compute_half_vertex(v1, v2, _radius, newV1);
//                _compute_half_vertex(v2, v3, _radius, newV2);
//                _compute_half_vertex(v1, v3, _radius, newV3);
//                _compute_half_tex_coords(t1, t2, newT1);
//                _compute_half_tex_coords(t2, t3, newT2);
//                _compute_half_tex_coords(t1, t3, newT3);
//                _compute_vertex_normal(newV1, newN1);
//                _compute_vertex_normal(newV2, newN2);
//                _compute_vertex_normal(newV3, newN3);
//
//                // add new vertices/normals/texcoords to arrays
//                // it will check if it is shared/non-shared and return index
//                newI1 = _add_subvertex_attribs(newV1, newN1, newT1);
//                newI2 = _add_subvertex_attribs(newV2, newN2, newT2);
//                newI3 = _add_subvertex_attribs(newV3, newN3, newT3);
//
//                // add 4 new triangle _indices
//                add_indices(i1, newI1, newI3);
//                add_indices(newI1, i2, newI2);
//                add_indices(newI1, newI2, newI3);
//                add_indices(newI3, newI2, i3);
//
//                // add new line _indices
//                _add_sub_line_indices(i1, newI1, i2, newI2, i3, newI3);
//            }
//        }
//    }
//
//    void IcosphereShape::_subdivide_vertices_flat() {
//
//        std::vector<float> temp_vertices;
//        std::vector<float> temp_tex_coords;
//        std::vector<unsigned int> temp_indices;
//
//        int index_count;
//        float *v1, *v2, *v3;
//        float *t1, *t2, *t3;
//        glm::vec3 new_V1, new_V2, new_V3;
//        glm::vec2 new_T1, new_T2, new_T3;
//        glm::vec3 normal;
//        unsigned int index = 0;
//        int i, j;
//
//        for(i = 0; i <= _subdivision; ++i){
//            temp_vertices = vertices;
//            temp_tex_coords = texture_coords;
//            temp_indices = _indices;
//
//            vertices.clear();
//            texture_coords.clear();
//            _indices.clear();
//            line_indices.clear();
//            normals.clear();
//
//            index = 0;
//            index_count = (int)temp_indices.size();
//            for(j=0; j < index_count; j += 3)
//            {
//                v1 = &temp_vertices[temp_indices[j] * 3];
//                v2 = &temp_vertices[temp_indices[j + 1] * 3];
//                v3 = &temp_vertices[temp_indices[j + 2] * 3];
//                t1 = &temp_tex_coords[temp_indices[j] * 2];
//                t2 = &temp_tex_coords[temp_indices[j + 1] * 2];
//                t3 = &temp_tex_coords[temp_indices[j + 2] * 2];
//
//                // get 3 new vertices
//                _compute_half_vertex(v1, v2, _radius, new_V1);
//                _compute_half_vertex(v2, v3, _radius, new_V2);
//                _compute_half_vertex(v1, v3, _radius, new_V3);
//                _compute_half_tex_coords(t1, t2, new_T1);
//                _compute_half_tex_coords(t2, t3, new_T2);
//                _compute_half_tex_coords(t1, t3, new_T3);
//
//                // add 4 new triangles
//                add_vertices(v1, new_V1, new_V3);
//                add_tex_coords(t1, new_T1, new_T3);
//                compute_face_normal(v1, new_V1, new_V3, normal);
//                add_normals(normal, normal, normal);
//                add_indices(index, index + 1, index + 2);
//
//                add_vertices(new_V1, v2, new_V2);
//                add_tex_coords(new_T1, t2, new_T2);
//                compute_face_normal(new_V1, v2, new_V2, normal);
//                add_normals(normal, normal, normal);
//                add_indices(index+3, index+4, index+5);
//
//                add_vertices(new_V1, new_V2, new_V3);
//                add_tex_coords(new_T1, new_T2, new_T3);
//                compute_face_normal(new_V1, new_V2, new_V3, normal);
//                add_normals(normal, normal, normal);
//                add_indices(index + 6, index + 7, index + 8);
//
//                add_vertices(new_V3, new_V2, v3);
//                add_tex_coords(new_T3, new_T2, t3);
//                compute_face_normal(new_V3, new_V2, v3, normal);
//                add_normals(normal, normal, normal);
//                add_indices(index + 9, index + 10, index + 11);
//
//                _add_sub_line_indices(index, index + 1, index + 4,
//                                      index + 5, index + 11, index + 9);
//
//                index += 12;
//            }
//        }
//    }
//
//
//
//    unsigned int IcosphereShape::_add_subvertex_attribs(const float *v, const float *n, const float *t)
//    {
//        unsigned int index;
//
//        // check if is shared vertex
//        if (IcosphereShape::_is_shared_texture_coord(t))
//        {
//            std::pair<float, float> key = std::make_pair(t[0], t[1]);
//            auto iter = _shared_indices.find(key);
//            if (iter == _shared_indices.end())
//            {
//                add_vertices(v[0], v[1], v[2]);
//                add_normals(n[0], n[1], n[2]);
//                add_tex_coords(t[0], t[1]);
//                index = texture_coords.size() / 2 - 1;
//                _shared_indices[key] = index;
//            }
//            else
//            {
//                index = iter->second;
//            }
//        }
//        // not shared index
//        else
//        {
//            add_vertices(v[0], v[1], v[2]);
//            add_normals(n[0], n[1], n[2]);
//            add_tex_coords(t[0], t[1]);
//            index = texture_coords.size() / 2 - 1;
//        }
//        return index;
//    }
//
//    void IcosphereShape::_compute_vertex_normal(const float *v, float *n)
//    {
//        glm::vec3 r = glm::normalize(glm::vec3(v[0], v[1], v[2]));
//        n[0] = r.x; n[1] = r.y; n[2] = r.z;
//    }
//
//    void IcosphereShape::_compute_half_tex_coords(const float *t1, const float *t2, float *nt)
//    {
//        glm::vec2 rv = (glm::vec2(t1[0], t1[1]) + glm::vec2(t2[0], t2[1])) / 2.f;
//        nt[0] = rv.x;
//        nt[1] = rv.y;
//    }
//
//    void IcosphereShape::_compute_half_tex_coords(const float *t1, const float *t2, glm::vec2& nt)
//    {
//        glm::vec2 rv = (glm::vec2(t1[0], t1[1]) + glm::vec2(t2[0], t2[1])) / 2.f;
//        nt.x = rv.x;
//        nt.y = rv.y;
//    }
//
//    std::vector<glm::vec3> IcosphereShape::_computeIcosahedronVertices() const
//    {
//        const float PI = 3.1415926f;
//        const float H_ANGLE = PI / 180 * 72;    // angle for each col
//        const float V_ANGLE = atanf(1.f / 2);   // elevation angle
//
//        std::vector<glm::vec3> vertices(12);    // 12 vertices
//        int i1, i2;                             // _indices
//        float z, xy;
//        // each row starts form a diferent angle, radians
//        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // -126 deg
//        float hAngle2 = -PI / 2;                // -90 deg
//
//        // top vertex
//        vertices[0] = glm::vec3(0, 0, _radius);
//
//        // 5 verts per row
//        for (int i = 1; i <= 5; i++)
//        {
//            i1 = i;
//            i2 = (i + 5); // 5 verts offset 3th row
//
//            z = _radius * sin(V_ANGLE);         // elevation
//            xy = _radius * cos(V_ANGLE);
//
//            vertices[i1] = glm::vec3(xy * cosf(hAngle1), xy * sinf(hAngle1), z);  // x
//            vertices[i2] = glm::vec3(xy * cosf(hAngle2), xy * sinf(hAngle2), -z);
//
//            // next horizontal angles
//            hAngle1 += H_ANGLE;
//            hAngle2 += H_ANGLE;
//        }
//
//        // bottom vertex
//        vertices[11] = glm::vec3(0, 0, -_radius);
//
//        return vertices;
//    }
//
//    bool IcosphereShape::_is_shared_texture_coord(const float *t)
//    {
//        // 20 non-shared line segments
//        const float S = 1.f / 11;
//        const float T = 1.f / 3;
//        static float segments[] = { S, 0,           0, T,       // 00 - 05
//                                    S, 0,           S * 2, T,     // 00 - 06
//                                    S * 3, 0,       S * 2, T,     // 01 - 06
//                                    S * 3, 0,       S * 4, T,     // 01 - 07
//                                    S * 5, 0,       S * 4, T,     // 02 - 07
//                                    S * 5, 0,       S * 6, T,     // 02 - 08
//                                    S * 7, 0,       S * 6, T,     // 03 - 08
//                                    S * 7, 0,       S * 8, T,     // 03 - 09
//                                    S * 9, 0,       S * 8, T,     // 04 - 09
//                                    S * 9, 0,       1, T * 2,     // 04 - 14
//                                    0, T,           S * 2, 1,     // 05 - 15
//                                    S * 3, T * 2,   S * 2, 1,     // 10 - 15
//                                    S * 3, T * 2,   S * 4, 1,     // 10 - 16
//                                    S * 5, T * 2,   S * 4, 1,     // 11 - 16
//                                    S * 5, T * 2,   S * 6, 1,     // 11 - 17
//                                    S * 7, T * 2,   S * 6, 1,     // 12 - 17
//                                    S * 7, T * 2,   S * 8, 1,     // 12 - 18
//                                    S * 9, T * 2,   S * 8, 1,     // 13 - 18
//                                    S * 9, T * 2,   S * 10, 1,    // 13 - 19
//                                    1, T * 2,       S * 10, 1 };  // 14 - 19
//
//        // check the point with all 20 line segments
//        // if it is on the line segment, it is non-shared
//        int count = (int)(sizeof(segments) / sizeof(segments[0]));
//        for (int i = 0, j = 2; i < count; i += 4, j += 4)
//        {
//            if (Shape::_is_on_line_segment(&segments[i], &segments[j], t))
//                return false;  // not shared
//        }
//
//        return true;
//    }


    // Sky box Shape
    // -------------

}
