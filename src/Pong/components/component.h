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
#include "Pong/serializer/serial_functions.h"

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


    struct RelationshipComponent : public Component {
        RegId parent{};
        std::vector<RegId> children{};
    };
    REG_COMPONENT(RelationshipComponent);
#define component_count_1 RelationshipComponent

    template<typename Archive>
    void serialize(Archive & ar, RelationshipComponent &relationship_component, const Version& version) {
        ar & relationship_component.parent;
        ar & relationship_component.children;
    }


    struct CameraComp : public Component {

        float fov{};

        explicit CameraComp(
                float fov = _P_PI / 3.5
        ) : fov(fov) {}
    };
    REG_COMPONENT(CameraComp);
#define component_count_2 CameraComp

    template<typename Archive>
    void serialize(Archive & ar, CameraComp &camera_comp, const Version& version) {
        ar & camera_comp.fov;
    }


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
#define component_count_3 TransformComp

    template<typename Archive>
    void serialize(Archive & ar, TransformComp & transform_comp, const Version &version) {
        ar & transform_comp.translation;
        ar & transform_comp.rotation;
        ar & transform_comp.scale;
        ar & transform_comp.visibility;
    }


    // TransformComp
    struct StaticMeshComp : public Component {
        /**Vertex array buffer id*/
        // files in disk
        RegId material{};
        RegId mesh{};
    };
    REG_COMPONENT(StaticMeshComp);
#define component_count_4 StaticMeshComp

    template<typename Archive>
    void serialize(Archive & ar, StaticMeshComp & static_mesh_comp, const Version &version) {
        ar & static_mesh_comp.material;
        ar & static_mesh_comp.mesh;
    }


    struct CubemapComp : public Component {
        RegId material{};
    };
    REG_COMPONENT(CubemapComp);
#define component_count_5 CubemapComp


    template<typename Archive>
    void serialize(Archive & ar, CubemapComp & cubemap_comp, const Version & version) {
        ar & cubemap_comp.material;
    }


    struct PythonComp : public Component {
        std::string script_path{};
        ParameterMap parameters{};
    };
    REG_COMPONENT(PythonComp);
#define component_count_6 PythonComp


    template<typename Archive>
    void serialize(Archive & ar, PythonComp & python_comp, const Version & version) {
        ar & python_comp.script_path;
    }


    struct LuaComp : public Component {
        std::string script_path{};
        ParameterMap parameters{};
    };
    REG_COMPONENT(LuaComp);
#define component_count_7 LuaComp


    template<typename Archive>
    void serialize(Archive & ar, LuaComp & lua_comp, const Version & version) {
        ar & lua_comp.script_path;
    }

}



#endif // PONG_SRC_PONG_COMPONENTS_COMPONENT_H_