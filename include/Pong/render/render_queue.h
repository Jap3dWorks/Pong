//
// Created by Jordi on 8/10/2022.
//

#ifndef GL_TEST_RENDER_QUEUE_H
#define GL_TEST_RENDER_QUEUE_H

#include <iostream>
#include <vector>
#include "Pong/registers/reg_id.h"
#include  "Pong/core/geometry_data.h"
#include "Pong/core/material.h"

#include <deque>
#include <functional>

namespace pong {

    struct RenderPiece {
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;

        std::vector<glm::mat4> transforms;
            // paint multiple times same mesh.
            // ID could be passed to render.

        RegId mesh;  // a mesh in memory

//        float* point_array;  // changes every frame
//        uint32_t point_size;
//        uint32_t* indices_val;
//        uint32_t indices_size;

        std::vector<Vertex> points;  // changes every frame
        std::vector<uint32_t> indices;

        GraphicPrimitives primitive{GraphicPrimitives::TRIANGLE};
        MaterialController material;  // shader + textures + values

        RenderPiece(RenderPiece&&) = default;
    };

    struct RenderChunk {
        std::vector<RenderPiece> render_pieces;

        void clear() noexcept {
            render_pieces.clear();
        }
    };

    using RenderQueue = std::deque<RenderChunk>;


    class RenderQueueSubmitter {
    public:
        using render_queue_reference = std::reference_wrapper<RenderQueue>;

    private:
        RenderChunk active_render_chunk;
        render_queue_reference render_queue_;

    public:
        void push_back(RenderPiece&& r_piece){
            active_render_chunk.render_pieces.push_back(
                    std::move(r_piece)
                    );
        }

        void submit() {
            render_queue_.get().push_back(std::move(active_render_chunk));
            active_render_chunk.clear();
        }
    };


    class RenderQueueConsumer {
    public:
        using render_queue_reference = std::reference_wrapper<RenderQueue>;

    private:
        uint32_t active_pos;
        render_queue_reference render_queue_;

        uint32_t viewers{0};

    public:
        RenderChunk active_render_chunk;

    public:
        void active_front() {
            active_render_chunk = std::move(render_queue_.get().front());
            render_queue_.get().pop_front();
        }

        void active_back() {
            active_render_chunk = std::move(render_queue_.get().back());
            render_queue_.get().pop_back();
        }

        void clear() noexcept {
            render_queue_.get().clear();
        }
    };
}

#endif //GL_TEST_RENDER_QUEUE_H
