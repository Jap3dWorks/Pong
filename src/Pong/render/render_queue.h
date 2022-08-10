//
// Created by Jordi on 8/10/2022.
//

#ifndef GL_TEST_RENDER_QUEUE_H
#define GL_TEST_RENDER_QUEUE_H

#include <iostream>
#include <vector>
#include "Pong/core/reg_id_manager.h"
#include <deque>
#include <functional>

namespace Pong {
    struct RenderPiece {
        RegId id; // Mesh Id
        size_t primitive; // hash of the primitive type
        // transforms, mesh, materials
        void* data;
    };

    struct RenderChunk {
        std::vector<RenderPiece> render_pieces_;

        void clean(){}
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
            active_render_chunk.render_pieces_.push_back(
                    std::move(r_piece)
                    );
        }

        void submit() {
            render_queue_.get().push_back(std::move(active_render_chunk));
            active_render_chunk.clean();
        }
    };


    class RenderQueueConsumer {
    public:
        using render_queue_reference = std::reference_wrapper<RenderQueue>;

    private:
        uint32_t active_pos;
        RenderChunk active_render_chunk;
        render_queue_reference render_queue_;

        void active_front() {
            active_render_chunk = std::move(render_queue_.get().front());
            render_queue_.get().pop_front();
        }

        void active_back() {
            active_render_chunk = std::move(render_queue_.get().back());
            render_queue_.get().pop_back();

        }
    };
}

#endif //GL_TEST_RENDER_QUEUE_H
