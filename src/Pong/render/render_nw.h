//
// Created by Jordi on 8/9/2022.
//

#ifndef GL_TEST_RENDER_NW_H
#define GL_TEST_RENDER_NW_H

#include "Pong/core/reg_id_manager.h"
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
        void render() override {}

        // transofrm component + camera component
//        std::vector<std::pair<RegId, RegId>> cameras;

        // transform comp + cam comp
//        std::vector<std::pair<RegId, RegId>> meshes;

        // camera + transform
        // geo + transform + material (shader and textures)



    };
}

#endif //GL_TEST_RENDER_NW_H
