//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIALIZER_H
#define GL_TEST_SERIALIZER_H


#include <iostream>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include <functional>
#include <optional>
#include <vector>

namespace Pong::serializer {
    struct ElemData {
        glm::mat4 transform;
        std::optional<RegId> parent;
        std::optional<RegId> shape;
        std::optional<RegId> material;
        std::optioanl<RegId> curve;
    };

    struct MeshData {
        RegId uid;
        Mesh mesh;
    };

    struct CurveData {
        RegId uid;
        Curve curve;
    };

    struct MaterialData {
        RegId uid;
        Material material;
    };


// Try to use reflection for struct serialization.
// https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application
template<typename T>
void serialize(Archive& ar, ElemData& elem_data,  const uint32_t version) {
    ar & elem_data.transform;
    ar & elem_data.parent;
    ar & elem_data.shape;
    ar & elem_data.material;
    ar & elem_data.curve;
}

    class AssetSerializer {
    public:
        template<typename T>
        using list_reference =  std::vector<std::reference_wrapper<T>>;

    public:
        list_reference<ElemData> elem_data;
        list_reference<MeshData> mesh_data;
        list_reference<CurveData> curve_data;
        list_reference<MaterialData> material_data;

        template<typename Archive>
        public serialize(Archive& ar, const uint32_t version) {
            ar & elem_data;
            ar & mesh_data;
            ar & curve_data;
            ar & material_data;

            ar & stops;
        }
    };


    BOOST_CLASS_VERSION(MeshSerializer, 0.0);

}

#endif //GL_TEST_SERIALIZER_H
