//
// Created by Jordi on 6/21/2022.
//

#ifndef GL_TEST_MESH_COMPACT_H
#define GL_TEST_MESH_COMPACT_H
#include "Pong/core/core_vals.h"
#include "Pong/core/primitive_component.h"
#include <iostream>
#include <algorithm>


namespace Pong{
    using VertexVector = std::vector<Vertex>;
    using IndexVector = std::vector<uint32_t>;

class MeshCompactData {
private:
    std::unique_ptr<uint8_t[]> _data;
    size_t _size{0};
    size_t _indices_offset{0};

private:
    _P_INLINE void _mesh_compact_data(const Vertex *vertices, const uint32_t &vertices_size,
                            const uint32_t *indices, const uint32_t &indices_size) {
        size_t block_size = vertices_size * sizeof(Vertex) +
                            indices_size * sizeof(uint32_t);

        auto ptr = std::make_unique<uint8_t[]>(block_size);

        std::memcpy(ptr.get(), vertices, vertices_size);
        std::memcpy(ptr.get() + vertices_size, indices, indices_size);

        _data = std::move(ptr);
        _size = block_size;
        _indices_offset = vertices_size;
    }


public:
    MeshCompactData(const Vertex *vertices, uint32_t vertices_size,
                    const uint32_t *indices, uint32_t indices_size) {
        _mesh_compact_data(vertices, vertices_size, indices, indices_size);
    }

    MeshCompactData(const VertexVector &vertices,
                    IndexVector indices) {
        _mesh_compact_data(vertices.data(), vertices.size(),
                           indices.data(), indices.size());
    }

    MeshCompactData(const MeshCompactData &other) = delete;

    MeshCompactData(MeshCompactData &&other) noexcept {
        _data = std::move(other._data);
        std::swap(_size, other._size);
        std::swap(_indices_offset, other._indices_offset);
    }

    ~MeshCompactData() = default;

    MeshCompactData& operator=(const MeshCompactData& other) = delete;

    MeshCompactData& operator=(MeshCompactData&& other)  noexcept {
        _data = std::move(other._data);
        std::swap(_size, other._size);
        std::swap(_indices_offset, other._indices_offset);

        return *this;
    }

    _P_NODISCARD const uint8_t *get() const {
        return _data.get();
    }

    _P_NODISCARD auto size() const {
        return _size;
    }

    _P_NODISCARD auto indices_offset() const {
        return _indices_offset;
    };

    _P_STATIC _P_INLINE IndexVector offset_indices(
            const size_t &vertex_offset, IndexVector &&indices) {

        std::for_each(indices.begin(), indices.end(),
                      [&](auto &n) {
                          n += vertex_offset;
                      }
        );

        return indices;
    }

    _P_STATIC _P_INLINE IndexVector offset_indices(
            const size_t &vertex_offset, IndexVector &indices) {

        std::for_each(indices.begin(), indices.end(),
                      [&](auto &n) {
                          n += vertex_offset;
                      }
        );

        return indices;
    }
};

}

#endif //GL_TEST_MESH_COMPACT_H
