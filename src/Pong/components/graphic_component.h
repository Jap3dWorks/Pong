#ifndef SHAPE_H
#define SHAPE_H

#include "Pong/core/geometry_data.h"
#include "Pong/components/component.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/graphic_flags.h"
#include "Pong/core/exceptions.h"
#include "Pong/map/reg_data.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <map>

#include <cstdarg>

namespace Pong {
    struct GraphicComponent : public Component {
    };

    // StaticMeshComponent
    struct StaticMeshComponent : public GraphicComponent {
        /**Vertex array buffer id*/
        // files in disk
        RegId material{};
        RegId mesh{};
    };

    struct CubemapComponent : public GraphicComponent {
        RegId material{};
    };
}

#endif // SHAPE_H
