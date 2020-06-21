#include "Shape.h"
#include "logger.h"

#include <stb_image.h>

#include <utility>


namespace Pong {

    // --Shape--
    // ---------
    Shape::Shape(std::string name) :_name(std::move(name)) {}

    const float* Shape::getInterleavedVertices() const
    { return _interleavedVertices.data(); }

    unsigned int Shape::getInterleavedVertexSize() const
    {return (unsigned int)_interleavedVertices.size() * sizeof(float);}

    unsigned int Shape::getInterleavedVertexCount() const
    { return getVertexCount(); }

    void Shape::_buildInterleavedVertices()
    {
        _interleavedVertices.clear();

        std::size_t i, j;
        std:size_t count = _vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            _interleavedVertices.push_back(_vertices[i]);
            _interleavedVertices.push_back(_vertices[i + 1]);
            _interleavedVertices.push_back(_vertices[i + 2]);

            _interleavedVertices.push_back(_normals[i]);
            _interleavedVertices.push_back(_normals[i + 1]);
            _interleavedVertices.push_back(_normals[i + 2]);

            _interleavedVertices.push_back(_tex_coords[j]);
            _interleavedVertices.push_back(_tex_coords[j + 1]);
        }
    }

    void Shape::setVAO()
    {
        int vRow = 0, nRow = 0, tRow = 0;
        if (getVertexCount())
            vRow = 3;
        if (getNormalCount())
            nRow = 3;
        if (getTexCoordsCount())
            tRow = 2;

        unsigned int VBO, EBO;
        // reset vertex array
        glGenVertexArrays(1, &VAO_id);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO_id);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
            getInterleavedVertexSize(),
            _interleavedVertices.data(),
            GL_STATIC_DRAW);

        // EBO object
        if (!_indices.empty()) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                getIndexSize(),
                _indices.data(),
                GL_STATIC_DRAW);
        }


        int rowSize = (vRow + nRow + tRow) * sizeof(float);
        int floatSize = sizeof(float);
        int offset = 0;
        int attrId = 0;

        // vertex positions attribute
        glEnableVertexAttribArray(attrId);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _interleavedStride,
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

    void Shape::draw() const
    {
        glBindVertexArray(VAO_id);
        // draw player
        if(_indices.size())
            glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
        // detach vertex array
        glBindVertexArray(0);
    }

    void Shape::compute_face_normal(
            const glm::vec3& vtx0, const glm::vec3& vtx1,
            const glm::vec3& vtx2, glm::vec3& out_normal)
    {
        const float EPSILON = 0.000001f;

        out_normal[0] = out_normal[1] = out_normal[2] = 0.f;

        // edges v0-v1 v0-v2
        glm::vec3 edge1 = vtx1 - vtx0;
        glm::vec3 edge2 = vtx2 - vtx0;

        // cross product
        out_normal = glm::cross(edge1, edge2);

        glm::normalize(out_normal);

    }

    void Shape::compute_face_normal(
            float* & vtx0, float * & vtx1,
            float* & vtx2, glm::vec3& out_normal)
    {
        compute_face_normal(
                glm::vec3(vtx0[0], vtx0[1],vtx0[2]),
                glm::vec3(vtx1[0], vtx1[1], vtx1[2]),
                glm::vec3(vtx2[0], vtx2[1], vtx2[2]),
                out_normal);
    }

    // --Icosphere--
    // -------------
    void IcosphereShape::buildIcosphere()
    {
        if (_smooth)
            _build_vertices_smooth();

        // set vertex buffers
        setVAO();

        // cout_buffer<float>(_interleavedVertices, 8);
    }

    IcosphereShape::IcosphereShape(std::string name, float radius, int subdivision, bool smooth):
        _radius(radius), _subdivision(subdivision), _smooth(smooth), Shape(std::move(name))
    {
        buildIcosphere();
    }

    void IcosphereShape::setRadius(float r)
    {
        _radius = r;
        buildIcosphere();

    }

    inline std::vector<glm::vec3> IcosphereShape::_prepare_icosphere_data()
    {
        _vertices.clear();
        _normals.clear();
        _tex_coords.clear();
        _indices.clear();

        return _computeIcosahedronVertices();
    }

    void IcosphereShape::_build_vertices_flat()
    {

        auto temp_vertex = _prepare_icosphere_data();


        glm::vec3 *v0, *v1, *v2, *v3, *v4, *v11;
        glm::vec3 normal;
        glm::vec2 t0, t1, t2, t3, t4, t11;
        unsigned int index = 0;

        //
        v0 = &temp_vertex[0];
        v11 = &temp_vertex[11];
        for(int i = 1; i<=5; ++i)
        {
            v1 = &temp_vertex[i];
            if(i<5){
                v2 = &temp_vertex[i + 1];
            }
            else{
                v2 = &temp_vertex[1];
            }

            v3 = &temp_vertex[i+5];
            if((i + 5) < 10){
                v4 = &temp_vertex[i + 6];
            }
            else{
                v4 = &temp_vertex[6];
            }

            // texture coords
            t0[0] = (2 * i - 1) * S_STEP; t0[1] = 0;
            t1[0] = (2 * i -2) * S_STEP;  t1[1] = T_STEP;
            t2[0] = (2 * i - 0) * S_STEP; t2[1] = T_STEP;
            t3[0] = (2 * i - 1) * S_STEP; t3[1] = T_STEP * 2;
            t4[0] = (2 * i + 1) * S_STEP; t4[1] = T_STEP * 2;
            t11[0] = 2 * i * S_STEP; t11[1] = T_STEP * 3;

            // triangles
            IcosphereShape::compute_face_normal(*v0, *v1, *v2, normal);
            add_vertices(*v0, *v1, *v2);
            add_normals(normal, normal, normal);
            add_tex_coords(t0, t1, t2);
            _add_indices(index, index + 1, index + 2);

            IcosphereShape::compute_face_normal(*v1, *v3, *v2, normal);
            add_vertices(*v1, *v3, *v2);
            add_normals(normal, normal, normal);
            add_tex_coords(t1, t3, t2);
            _add_indices(index + 3, index + 4, index + 5);

            IcosphereShape::compute_face_normal(*v2, *v3, *v4, normal);
            add_vertices(*v2, *v3, *v4);
            add_normals(normal, normal, normal);
            add_tex_coords(t1, t3, t4);
            _add_indices(index + 6, index + 7, index + 8);

            IcosphereShape::compute_face_normal(*v3, *v11, *v4, normal);
            add_vertices(*v3, *v11, *v4);
            add_normals(normal, normal, normal);
            add_tex_coords(t3, t11, t4);
            _add_indices(index + 9, index + 10, index + 11);

            // edge lines
            _lineIndices.push_back(index);  //
            _lineIndices.push_back(index + 1);
            _lineIndices.push_back(index + 3); //
            _lineIndices.push_back(index + 4);
            _lineIndices.push_back(index + 3); //
            _lineIndices.push_back(index + 5);
            _lineIndices.push_back(index + 4); //
            _lineIndices.push_back(index + 5);
            _lineIndices.push_back(index + 9); //
            _lineIndices.push_back(index + 10);
            _lineIndices.push_back(index + 9); //
            _lineIndices.push_back(index + 11);

            index += 12;
        }

        _subdivide_vertices_flat();

        _buildInterleavedVertices();
    }

    void IcosphereShape::_build_vertices_smooth()
    {

        // compute de basic 12 vertices of a icosaedron
        // vertices with different texcoords cannot be shared.
        auto tmpVert = _prepare_icosphere_data();

        // smooth icosahedron has 14 non-shared vertices
        // and 8 shared vertices total 22 vertices.
        for (int i = 0; i < 5; i++)                        // v0, v1, v2, v3, v4 top
        {
            add_vertices(tmpVert[0]);
            add_normals(0.f, 0.f, 1.f);
            add_tex_coords(S_STEP + (S_STEP * (i * 2)), 0.f);
        }

        for (int i = 0; i < 5; i++)                       // v5, v6, v7, v8, v9 bottom
        {
            add_vertices(tmpVert[11]);
            add_normals(0.f, 0.f, -1.f);
            add_tex_coords(S_STEP * 2 + (S_STEP * (i * 2)), T_STEP * 3);
        }

        for (int i = 0; i < 2; i++) {                     // v10 right, v11 left
            add_vertices(tmpVert[1]);
            add_normals(glm::normalize(tmpVert[1]));
            add_tex_coords(S_STEP * i * 10, T_STEP);
        }

        for (int i = 0; i < 2; i++) {                     // v12 right, v13 left
            add_vertices(tmpVert[6]);
            add_normals(glm::normalize(tmpVert[6]));
            add_tex_coords(S_STEP + (S_STEP * i * 10), T_STEP * 2);
        }

        // 8 shared vertices
        for (int i = 0; i < 4; i++)                       // v14, v15, v16, v17 shared
        {
            add_vertices(tmpVert[i + 2]);
            add_normals(glm::normalize(tmpVert[i + 2]));
            add_tex_coords(S_STEP * ((i + 1) * 2), T_STEP);
            _shared_indices[std::make_pair(S_STEP * ((i + 1) * 2), T_STEP)] = _tex_coords.size() / 2 - 1;

        }

        for (int i = 0; i < 4; i++)
        {
            add_vertices(tmpVert[i + 7]);                     // v18, v19, v20, v21 shared
            add_normals(glm::normalize(tmpVert[i + 7]));
            add_tex_coords(S_STEP * 3 + (S_STEP * i * 2), T_STEP * 2);
            _shared_indices[std::make_pair(S_STEP * 3 + (S_STEP * i * 2), T_STEP * 2)] = _tex_coords.size() / 2 - 1;
        }

        // build triangles index list (20 triangles)
        _add_indices(0, 10, 14);     // 1st row
        _add_indices(1, 14, 15);
        _add_indices(2, 15, 16);
        _add_indices(3, 16, 17);
        _add_indices(4, 17, 11);
        _add_indices(10, 12, 14);    // 2nd row
        _add_indices(12, 18, 14);
        _add_indices(14, 18, 15);
        _add_indices(18, 19, 15);
        _add_indices(15, 19, 16);
        _add_indices(19, 20, 16);
        _add_indices(16, 20, 17);
        _add_indices(20, 21, 17);
        _add_indices(17, 21, 11);
        _add_indices(21, 13, 11);
        _add_indices(5, 18, 12);  // 3rd row
        _add_indices(6, 19, 18);
        _add_indices(7, 20, 19);
        _add_indices(8, 21, 20);
        _add_indices(9, 13, 21);

        // add edge lines of icosahedron
        _lineIndices.push_back(0);   _lineIndices.push_back(10); // 00 - 10
        _lineIndices.push_back(1);   _lineIndices.push_back(14); // 01 - 14
        _lineIndices.push_back(2);   _lineIndices.push_back(15); // 02 - 15
        _lineIndices.push_back(3);   _lineIndices.push_back(16); // 03 - 16
        _lineIndices.push_back(4);   _lineIndices.push_back(17); // 04 - 17
        _lineIndices.push_back(10);  _lineIndices.push_back(14); // 10 - 14
        _lineIndices.push_back(14);  _lineIndices.push_back(15); // 14 - 15
        _lineIndices.push_back(15);  _lineIndices.push_back(16); // 15 - 16
        _lineIndices.push_back(16);  _lineIndices.push_back(17); // 10 - 14
        _lineIndices.push_back(17);  _lineIndices.push_back(11); // 17 - 11
        _lineIndices.push_back(10);  _lineIndices.push_back(12); // 10 - 12
        _lineIndices.push_back(12);  _lineIndices.push_back(14); // 12 - 14
        _lineIndices.push_back(14);  _lineIndices.push_back(18); // 14 - 18
        _lineIndices.push_back(18);  _lineIndices.push_back(15); // 18 - 15
        _lineIndices.push_back(15);  _lineIndices.push_back(19); // 15 - 19
        _lineIndices.push_back(19);  _lineIndices.push_back(16); // 19 - 16
        _lineIndices.push_back(16);  _lineIndices.push_back(20); // 16 - 20
        _lineIndices.push_back(20);  _lineIndices.push_back(17); // 20 - 17
        _lineIndices.push_back(17);  _lineIndices.push_back(21); // 17 - 21
        _lineIndices.push_back(21);  _lineIndices.push_back(11); // 21 - 11
        _lineIndices.push_back(12);  _lineIndices.push_back(18); // 12 - 18
        _lineIndices.push_back(18);  _lineIndices.push_back(19); // 18 - 19
        _lineIndices.push_back(19);  _lineIndices.push_back(20); // 19 - 20
        _lineIndices.push_back(20);  _lineIndices.push_back(21); // 20 - 21
        _lineIndices.push_back(21);  _lineIndices.push_back(13); // 21 - 13
        _lineIndices.push_back(5);   _lineIndices.push_back(12); // 05 - 12
        _lineIndices.push_back(6);   _lineIndices.push_back(18); // 06 - 18
        _lineIndices.push_back(7);   _lineIndices.push_back(19); // 07 - 19
        _lineIndices.push_back(8);   _lineIndices.push_back(20); // 08 - 20
        _lineIndices.push_back(9);   _lineIndices.push_back(21); // 09 - 21

        // subdivide icosahedron
        _subdivide_vertices_smooth();


        // gen interleaved vertex array, buffer open gl will use.
        _buildInterleavedVertices();

    }

    void IcosphereShape::_subdivide_vertices_smooth()
    {
        std::vector<unsigned int> tmpIndices;
        int indexCount;
        unsigned int i1, i2, i3;    // indices from original triangle
        const float *v1, *v2, *v3;  // ptr to original vertices of a triangle
        const float *t1, *t2, *t3;  // ptr to original texcorrds of a triangle
        float newV1[3], newV2[3], newV3[3];
        float newN1[3], newN2[3], newN3[3];
        float newT1[3], newT2[3], newT3[3];
        unsigned int newI1, newI2, newI3;
        int i, j;

        // iteration for subdivision
        for (i = 1; i <= _subdivision; ++i)
        {
            // copy prev indices
            tmpIndices = _indices;

            // clear prev arrays
            _indices.clear();
            _lineIndices.clear();

            indexCount = (int)tmpIndices.size();
            for (j = 0; j < indexCount; j += 3)
            {
                // get 3 indices each triangle
                i1 = tmpIndices[j];
                i2 = tmpIndices[j + 1];
                i3 = tmpIndices[j + 2];

                // get 3 vertex attribs from prev triangle
                v1 = &_vertices[i1 * 3];
                v2 = &_vertices[i2 * 3];
                v3 = &_vertices[i3 * 3];
                t1 = &_tex_coords[i1 * 2];
                t2 = &_tex_coords[i2 * 2];
                t3 = &_tex_coords[i3 * 2];

                // get 3 new vertex attribs by splitting half on each edge
                compute_half_vertex(v1, v2, _radius, newV1);
                compute_half_vertex(v2, v3, _radius, newV2);
                compute_half_vertex(v1, v3, _radius, newV3);
                compute_half_tex_coords(t1, t2, newT1);
                compute_half_tex_coords(t2, t3, newT2);
                compute_half_tex_coords(t1, t3, newT3);
                _computeVertexNormal(newV1, newN1);
                _computeVertexNormal(newV2, newN2);
                _computeVertexNormal(newV3, newN3);

                // add new vertices/normals/texcoords to arrays
                // it will check if it is shared/non-shared and return index
                newI1 = _addSubVertexAttribs(newV1, newN1, newT1);
                newI2 = _addSubVertexAttribs(newV2, newN2, newT2);
                newI3 = _addSubVertexAttribs(newV3, newN3, newT3);

                // add 4 new triangle indices
                _add_indices(i1, newI1, newI3);
                _add_indices(newI1, i2, newI2);
                _add_indices(newI1, newI2, newI3);
                _add_indices(newI3, newI2, i3);

                // add new line indices
                _add_sub_line_indices(i1, newI1, i2, newI2, i3, newI3);
            }
        }
    }

    void IcosphereShape::_subdivide_vertices_flat() {

        std::vector<float> temp_vertices;
        std::vector<float> temp_tex_coords;
        std::vector<unsigned int> temp_indices;

        int index_count;
        float *v1, *v2, *v3;
        float *t1, *t2, *t3;
        glm::vec3 new_V1, new_V2, new_V3;
        glm::vec2 new_T1, new_T2, new_T3;
        glm::vec3 normal;
        unsigned int index = 0;
        int i, j;

        for(i = 0; i <= _subdivision; ++i){
            temp_vertices = _vertices;
            temp_tex_coords = _tex_coords;
            temp_indices = _indices;

            _vertices.clear();
            _tex_coords.clear();
            _indices.clear();
            _lineIndices.clear();
            _normals.clear();

            index = 0;
            index_count = (int)temp_indices.size();
            for(j=0; j < index_count; j += 3)
            {
                v1 = &temp_vertices[temp_indices[j] * 3];
                v2 = &temp_vertices[temp_indices[j + 1] * 3];
                v3 = &temp_vertices[temp_indices[j + 2] * 3];
                t1 = &temp_tex_coords[temp_indices[j] * 2];
                t2 = &temp_tex_coords[temp_indices[j + 1] * 2];
                t3 = &temp_tex_coords[temp_indices[j + 2] * 2];

                // get 3 new vertices
                compute_half_vertex(v1, v2, _radius, new_V1);
                compute_half_vertex(v2, v3, _radius, new_V2);
                compute_half_vertex(v1, v3, _radius, new_V3);
                compute_half_tex_coords(t1, t2, new_T1);
                compute_half_tex_coords(t2, t3, new_T2);
                compute_half_tex_coords(t1, t3, new_T3);

                // add 4 new triangles
                add_vertices(v1, new_V1, new_V3);
                add_tex_coords(t1, new_T1, new_T3);
                compute_face_normal(v1, new_V1, new_V3, normal);

            }

        }

    }

    void IcosphereShape::_add_sub_line_indices(unsigned int i1, unsigned int i2, unsigned int i3,
                                               unsigned int i4, unsigned int i5, unsigned int i6)
    {
        _lineIndices.push_back(i1);      // i1 - i2
        _lineIndices.push_back(i2);
        _lineIndices.push_back(i2);      // i2 - i6
        _lineIndices.push_back(i6);
        _lineIndices.push_back(i2);      // i2 - i3
        _lineIndices.push_back(i3);
        _lineIndices.push_back(i2);      // i2 - i4
        _lineIndices.push_back(i4);
        _lineIndices.push_back(i6);      // i6 - i4
        _lineIndices.push_back(i4);
        _lineIndices.push_back(i3);      // i3 - i4
        _lineIndices.push_back(i4);
        _lineIndices.push_back(i4);      // i4 - i5
        _lineIndices.push_back(i5);
    }


    unsigned int IcosphereShape::_addSubVertexAttribs(const float v[3], const float n[3], const float t[2])
    {
        // TODO: change to glm::vecX
        unsigned int index;

        // check if is shared vertex
        if (IcosphereShape::_isSharedTexCoord(t))
        {
            std::pair<float, float> key = std::make_pair(t[0], t[1]);
            auto iter = _shared_indices.find(key);
            if (iter == _shared_indices.end())
            {
                add_vertices(v[0], v[1], v[2]);
                add_normals(n[0], n[1], n[2]);
                add_tex_coords(t[0], t[1]);
                index = _tex_coords.size() / 2 - 1;
                _shared_indices[key] = index;
            }
            else
            {
                index = iter->second;
            }
        }
        // not shared index
        else
        {
            add_vertices(v[0], v[1], v[2]);
            add_normals(n[0], n[1], n[2]);
            add_tex_coords(t[0], t[1]);
            index = _tex_coords.size() / 2 - 1;
        }
        return index;
    }

    void IcosphereShape::_computeVertexNormal(const float v[3], float n[3])
    {
        glm::vec3 r = glm::normalize(glm::vec3(v[0], v[1], v[2]));
        n[0] = r.x; n[1] = r.y; n[2] = r.z;
    }

    void IcosphereShape::compute_half_tex_coords(const float *t1, const float *t2, float *nt)
    {
        glm::vec2 rv = (glm::vec2(t1[0], t1[1]) + glm::vec2(t2[0], t2[1])) / 2.f;
        nt[0] = rv.x;
        nt[1] = rv.y;
    }

    void IcosphereShape::compute_half_tex_coords(const float *t1, const float *t2, glm::vec2& nt)
    {
        glm::vec2 rv = (glm::vec2(t1[0], t1[1]) + glm::vec2(t2[0], t2[1])) / 2.f;
        nt.x = rv.x;
        nt.y = rv.y;
    }

    void IcosphereShape::compute_half_vertex(const float *v1, const float *v2, float length, float *nv)
    {
        glm::vec3 rVec = glm::normalize(glm::vec3(v1[0],v1[1],v1[2]) + glm::vec3(v2[0],v2[1],v2[2]));
        nv[0] = rVec.x;
        nv[1] = rVec.y;
        nv[2] = rVec.z;
    }

    void IcosphereShape::compute_half_vertex(const float v1[3], const float v2[3], float& length, glm::vec3& nv)
    {
        glm::vec3 rVec = glm::normalize(glm::vec3(v1[0],v1[1],v1[2]) + glm::vec3(v2[0],v2[1],v2[2]));
        nv.x = rVec.x;
        nv.y = rVec.y;
        nv.z = rVec.z;
    }

    std::vector<glm::vec3> IcosphereShape::_computeIcosahedronVertices()
    {
        const float PI = 3.1415926f;
        const float H_ANGLE = PI / 180 * 72;    // angle for each col
        const float V_ANGLE = atanf(1.f / 2);   // elevation angle

        std::vector<glm::vec3> vertices(12);    // 12 vertices
        int i1, i2;                             // indices
        float z, xy;
        // each row starts form a diferent angle, radians
        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // -126 deg
        float hAngle2 = -PI / 2;                // -90 deg

        // top vertex
        vertices[0] = glm::vec3(0, 0, _radius);

        // 5 verts per row
        for (int i = 1; i <= 5; i++)
        {
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

    bool IcosphereShape::_isSharedTexCoord(const float t[2])
    {
        // 20 non-shared line segments
        const float S = 1.f / 11;
        const float T = 1.f / 3;
        static float segments[] = { S, 0,           0, T,       // 00 - 05
                                    S, 0,           S * 2, T,     // 00 - 06
                                    S * 3, 0,       S * 2, T,     // 01 - 06
                                    S * 3, 0,       S * 4, T,     // 01 - 07
                                    S * 5, 0,       S * 4, T,     // 02 - 07
                                    S * 5, 0,       S * 6, T,     // 02 - 08
                                    S * 7, 0,       S * 6, T,     // 03 - 08
                                    S * 7, 0,       S * 8, T,     // 03 - 09
                                    S * 9, 0,       S * 8, T,     // 04 - 09
                                    S * 9, 0,       1, T * 2,     // 04 - 14
                                    0, T,           S * 2, 1,     // 05 - 15
                                    S * 3, T * 2,   S * 2, 1,     // 10 - 15
                                    S * 3, T * 2,   S * 4, 1,     // 10 - 16
                                    S * 5, T * 2,   S * 4, 1,     // 11 - 16
                                    S * 5, T * 2,   S * 6, 1,     // 11 - 17
                                    S * 7, T * 2,   S * 6, 1,     // 12 - 17
                                    S * 7, T * 2,   S * 8, 1,     // 12 - 18
                                    S * 9, T * 2,   S * 8, 1,     // 13 - 18
                                    S * 9, T * 2,   S * 10, 1,    // 13 - 19
                                    1, T * 2,       S * 10, 1 };  // 14 - 19

        // check the point with all 20 line segments
        // if it is on the line segment, it is non-shared
        int count = (int)(sizeof(segments) / sizeof(segments[0]));
        for (int i = 0, j = 2; i < count; i += 4, j += 4)
        {
            if (IcosphereShape::_isOnLineSegment(&segments[i], &segments[j], t))
                return false;  // not shared
        }

        return true;
    }

    bool IcosphereShape::_isOnLineSegment(const float a[2], const float b[2], const float c[2])
    {
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

    IcosphereShape::~IcosphereShape() = default;

    // --Cube--
    // --------
    CubeShape::CubeShape(std::string name, float height, float width, float depth):
        _width(width), _height(height), _depth(depth), Shape(std::move(name))
    {
        _buildCubeVerticesHard();

        // set vertex buffers
        setVAO();

        //cout_buffer<float>(_interleavedVertices, 8);
    }

    CubeShape::~CubeShape() = default;

    void CubeShape::_buildCubeVerticesHard()
    {
        //const float S_STEP = 186 / 2048.f; // horizontal texture step
        //const float T_STEP = 322 / 1024.f; // vertical texture step

        // compute de basic 8 vertices of a cube
        // vertices with different texcoords cannot be shared.
        std::vector<glm::vec3> tmpVert = _computeCubeVertices();

        // clear prev arrays
        _vertices.clear();
        _normals.clear();
        _tex_coords.clear();
        _indices.clear();
        _sharedIndices.clear();

        // face cube has 12 non-shared vertices
        // and 12 shared vertices total 24 vertices.
        glm::vec2 fUv[4] = { glm::vec2(1.f, 0.f), glm::vec2(1.f, 1.f),
            glm::vec2(0.f, 1.f), glm::vec2(0.f, 0.f) };
        // face 1
        glm::vec3 fNr(0.f, 1.f, 0.f);
        add_vertices(tmpVert[0]);                      // v0 shared
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 0;

        add_vertices(tmpVert[1]);                      // v1
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[2]);                      // v2 shared
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 2;

        add_vertices(tmpVert[3]);                      // v3
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        //face 2
        fNr = glm::vec3(1.f, 0.f, 0.f);
        add_vertices(tmpVert[5]);                      // v4
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 4;

        add_vertices(tmpVert[1]);                      // v5
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[0]);                      // v6
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 6;

        add_vertices(tmpVert[4]);                      // v7
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        //face 3
        fNr = glm::vec3(0.f, 0.f, -1.f);
        add_vertices(tmpVert[6]);                     // v8
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 8;

        add_vertices(tmpVert[2]);                     // v9
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[1]);                     // v10
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 10;

        add_vertices(tmpVert[5]);                     // v11
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        //face 4
        fNr = glm::vec3(-1.f, 0.f, 0.f);
        add_vertices(tmpVert[7]);                     // v12
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 12;

        add_vertices(tmpVert[3]);                     // v13
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[2]);                     // v14
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 14;

        add_vertices(tmpVert[6]);                     // v15
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        //face 5
        fNr = glm::vec3(0.f, 0.f, 1.f);
        add_vertices(tmpVert[4]);                     // v16
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 16;

        add_vertices(tmpVert[0]);                     // v17
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[3]);                     // v18
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 18;

        add_vertices(tmpVert[7]);                     // v19
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        //face 6
        fNr = glm::vec3(0.f, -1.f, 0.f);
        add_vertices(tmpVert[4]);                     // v20
        add_normals(fNr);
        add_tex_coords(fUv[0]);
        //_shared_indices[std::make_pair(fNr, fUv[0])] = 20;

        add_vertices(tmpVert[5]);                     // v21
        add_normals(fNr);
        add_tex_coords(fUv[1]);

        add_vertices(tmpVert[6]);                     // v22
        add_normals(fNr);
        add_tex_coords(fUv[2]);
        //_shared_indices[std::make_pair(fNr, fUv[2])] = 22;

        add_vertices(tmpVert[7]);                     // v23
        add_normals(fNr);
        add_tex_coords(fUv[3]);

        // triangle index list 12 triangles
        _add_indices(0, 1, 2);  // top face
        _add_indices(0, 2, 3);
        _add_indices(4, 5, 6);  // face 2
        _add_indices(4, 6, 7);
        _add_indices(8, 9, 10);  // face 3
        _add_indices(8, 10, 11);
        _add_indices(12, 13, 14);  // face 4
        _add_indices(12, 14, 15);
        _add_indices(16, 17, 18);  // face 5
        _add_indices(16, 18, 19);
        _add_indices(20, 21, 22);  // bottom face
        _add_indices(20, 22, 23);

        // add edge lines, for now it is not necessary.
        // TODO: when implement subdivision, implement this too

        // gen interleaved vertex array, buffer open gl will use.
        _buildInterleavedVertices();
    }

    std::vector<glm::vec3> CubeShape::_computeCubeVertices() const
    {
        std::vector<glm::vec3> tmpVert(8);

        // uper vertices
        tmpVert[0] = glm::vec3(_width / 2, _height / 2, _depth / 2);
        tmpVert[1] = glm::vec3(_width / 2, _height / 2, -_depth / 2);
        tmpVert[2] = glm::vec3(-_width / 2, _height / 2, -_depth / 2);
        tmpVert[3] = glm::vec3(-_width / 2, _height / 2, _depth / 2);

        // lower vertices
        tmpVert[4] = glm::vec3(_width / 2, -_height / 2, _depth / 2);
        tmpVert[5] = glm::vec3(_width / 2, -_height / 2, -_depth / 2);
        tmpVert[6] = glm::vec3(-_width / 2, -_height / 2, -_depth / 2);
        tmpVert[7] = glm::vec3(-_width / 2, -_height / 2, _depth / 2);

        return tmpVert;
    }
}