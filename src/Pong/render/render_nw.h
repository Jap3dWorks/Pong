//
// Created by Jordi on 8/9/2022.
//

#ifndef GL_TEST_RENDER_NW_H
#define GL_TEST_RENDER_NW_H

#include "Pong/core/reg_id_manager.h"
#include "Pong/core/graphic_flags.h"
#include "Pong/map/map_level.h"
#include "Pong/render/render_queue.h"
#include <vector>
#include <queue>
#include <functional>


namespace Pong {

    class RenderManager {
        // create renderers queues and connect to submitters and renderers
    };


    class Renderer {
    public:
        using queue_consumer_reference = std::reference_wrapper<RenderQueueConsumer>;
    protected:
        queue_consumer_reference queue_consumer_;

    private:
        virtual void render() {}

    public:
        void set_render_queue(queue_consumer_reference render_queue) {
            queue_consumer_ = render_queue;
        }
    };


    class OglRenderer: public Renderer {
    private:
        void render() override {
            queue_consumer_.get().active_back();
            for (auto& rpiece: queue_consumer_.get().active_render_chunk.render_pieces) {
                if(rpiece.primitive == GraphicPrimitives::CAMERA) {
                    // update camera layouts
                    update_camera(rpiece);
                }
                else if (rpiece.primitive == GraphicPrimitives::TRIANGLE) {
                    draw_triangles(rpiece);
                }
            }
        }

        void update_camera(const RenderPiece& rpiece) {

        }

        void draw_triangles(const RenderPiece& rpiece) {
            // OpenGl draw implementation here ...
        }

        // transofrm component + camera component
//        std::vector<std::pair<RegId, RegId>> cameras;

        // transform comp + cam comp
//        std::vector<std::pair<RegId, RegId>> meshes;

        // camera + transform
        // geo + transform + material (shader and textures)



    };
}

#endif //GL_TEST_RENDER_NW_H
