//
// Created by Jordi on 8/9/2022.
//

#ifndef GL_TEST_RENDER_NW_H
#define GL_TEST_RENDER_NW_H

#include "Pong/map/reg_id_manager.h"
#include "Pong/map/map_level.h"
#include <vector>
#include <queue>

namespace Pong {
    class RenderNw {
    private:
        const MapLevel* _map_level;


    private:
        // transofrm component + camera component
        std::vector<std::pair<RegId, RegId>> cameras;

        // transform comp + cam comp
        std::vector<std::pair<RegId, RegId>> meshes;

        // camera + transform
        // geo + transform + material (shader and textures)



    };
}

#endif //GL_TEST_RENDER_NW_H
