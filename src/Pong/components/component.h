#ifndef PONG_SRC_PONG_COMPONENTS_COMPONENT_H_
#define PONG_SRC_PONG_COMPONENTS_COMPONENT_H_

//#include "Pong/core/entity/entity.h"
//#include "Pong/core/collider.h"
//#include "Utils/subclasses_map.h"
//#include "Utils/fixed_address_buffer.h"
#include "Utils/logger.h"
#include "Pong/math_utils.h"
#include "Pong/core/core_vals.h"
#include "Pong/core/parameter_map.h"
#include "Pong/components/component_hasher.h"
#include "Pong/registers/reg_id.h"

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
#include <unordered_map>


namespace pong::component {
    /**
     *  Component class, an Actor has a list of components, each Component
     *  can contain a custom script you only have to inherit from Component.
    */

    struct Component {
        RegId actor{};

        virtual inline void start() {};
        virtual inline void update() {};

    };


    struct CameraComp : public Component {

        float fov{};

        explicit CameraComp(
                float fov = _P_PI / 3.5
        ) : fov(fov) {}
    };
    REG_COMPONENT(CameraComp);
#define component_count_1


    struct TransformComp : Component {
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
    REG_COMPONENT(TransformComp);
#define component_count_2


    // TransformComp
    struct StaticMeshComp : public Component {
        /**Vertex array buffer id*/
        // files in disk
        RegId material{};
        RegId mesh{};
    };
    REG_COMPONENT(StaticMeshComp);
#define component_count_3


    struct CubemapComp : public Component {
        RegId material{};
    };
    REG_COMPONENT(CubemapComp);
#define component_count_4


    struct PythonComp : public Component {
        std::string script_path{};
        ParameterMap parameters{};
    };
    REG_COMPONENT(PythonComp);
#define component_count_5


    struct LuaComp : public Component {
        std::string script_path{};
        ParameterMap parameters{};
    };
    REG_COMPONENT(LuaComp);
#define component_count_6 // TODO: change the counter method


}



#endif // PONG_SRC_PONG_COMPONENTS_COMPONENT_H_