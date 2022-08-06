//
// Created by Jordi on 6/13/2022.
//

#ifndef PONG_GRAPHIC_FLAGS_H
#define PONG_GRAPHIC_FLAGS_H

namespace Pong {
    enum class RenderLayer : uint32_t {
        NO_LAYER = 0,
        BASE = 1,
        SKY_BOX = 2,
        BLENDING = 4
    };

    enum class UboLayouts : uint32_t {
        VIEW_MATRICES = 0,
        LIGHTS = 1,
        RUNTIME_DATA = 2,  // frame, fps, time, delta_time
        RENDER_DATA = 3,
    };

    enum class VertexAttrId : uint32_t {
        POSITION = 0,
        NORMAL = 1,
        TEX_COORDS = 2
    };

    enum class GraphicComponentType : uint32_t {
        STATIC = 0,
        DYNAMIC = 1
    };

    enum class GraphicPrimitives: uint32_t {
        POINT=0,
        LINE=1,
        TRIANGLE=2
    };

}

#endif //PONG_GRAPHIC_FLAGS_H
