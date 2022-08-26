#ifndef PONG_SRC_PONG_COMPONENTS_COMPONENT_H_
#define PONG_SRC_PONG_COMPONENTS_COMPONENT_H_

//#include "Pong/core/actor/actor.h"
//#include "Pong/core/collider.h"
//#include "Utils/subclasses_map.h"
//#include "Utils/fixed_address_buffer.h"
#include "Utils/logger.h"
#include "Pong/math_utils.h"
#include "Pong/core/core_vals.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <vector>
#include <cassert>
#include <concepts>
#include <string>
#include <iostream>
#include <algorithm>

namespace pong {
    /**
     *  Component class, an Actor has a list of components, each Component
     *  can contain a custom script you only have to inherit from Component.
     *
    */

    struct Component {
        RegId actor{};

        virtual inline void start() {};
        virtual inline void update() {};

    };

    struct CameraComponent : public Component {

        float fov{_P_PI / 3.5};

        explicit CameraComponent(
                float fov = _P_PI / 3.5
        ) :
                fov(fov) {}
    };

    struct TransformComponent : Component {
        glm::vec3 translation{0.0};
        glm::vec3 rotation{0.0};
        glm::vec3 scale{1.0};
        bool visibility{true};

        [[nodiscard]] glm::mat4 get_transform() const {
            return glm::translate(glm::mat4(1.0f), translation)
                   * glm::toMat4(glm::quat(rotation))
                   * glm::scale(glm::mat4(1.0f), scale);
        }

        void set_transform(const glm::mat4 &transform) {
            math::decompose_transforms(transform, translation, rotation, scale);
        }
    };

    // StaticMeshComponent
    struct StaticMeshComponent : public Component {
        /**Vertex array buffer id*/
        // files in disk
        RegId material{};
        RegId mesh{};
    };

    struct CubemapComponent : public Component {
        RegId material{};
    };

}



#endif // PONG_SRC_PONG_COMPONENTS_COMPONENT_H_