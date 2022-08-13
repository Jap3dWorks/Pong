//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIALIZER_H
#define GL_TEST_SERIALIZER_H


#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include "Pong/file_data/reflectable.h"

#include <iostream>
#include <vector>
#include <boost/archive/text_iarchive.hpp>
#include <functional>
#include <optional>
#include <vector>

#define SERIALIZABLE REFLECTABLE

namespace Pong::serializer {
    struct ElemData {
        SERIALIZABLE(
            EXPOSE(glm::mat4, transform, 1),
            EXPOSE(std::optional<RegId>, parent),
            EXPOSE(std::optional<RegId>, shape),
            EXPOSE(std::optional<RegId>, material),
            EXPOSE(std::optional<RegId>, curve)
        )
    };

    struct MeshData {
        SERIALIZABLE(
                EXPOSE(RegId, uid),
                EXPOSE(Mesh, mesh)
        )
    };

    struct CurveData {
        SERIALIZABLE (
            EXPOSE(RegId, uid),
            EXPOSE(Curve, curve)
        )
    };

    struct MaterialData {
        SERIALIZABLE (
                EXPOSE(RegId, uid),
                EXPOSE(Material, material)
        )
    };

// Try to use reflection for struct serialization.
// https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application
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
        void serialize(Archive & ar, const uint32_t version) const {
            ar & elem_data;
            ar & mesh_data;
            ar & curve_data;
            ar & material_data;
        }

        template<typename Archive>
        void load(Archive & ar, const unsigned int version) {
            ar & elem_data;
            ar & mesh_data;
            ar & curve_data;
            ar & material_data;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER();

    };


    struct serialize_visitor {
        AssetSerializer &asset_serializer;

        explicit serialize_visitor(AssetSerializer& asset_serializer_):
        asset_serializer(asset_serializer_) {}

        template<typename FieldData>
        void operator()(FieldData f) {
            asset_serializer & f.get();
        }
    };

    template<typename T>
    void serialize_fields(T& x, AssetSerializer& asset_serializer) {
        visit_each(x, serialize_visitor(asset_serializer));
    }

}


namespace boost::serialization {
//    template<typename Archive>
//    void serialize(Archive &ar, Pong::serializer::ElemData &elem_data, const uint32_t version) {
//        ar & elem_data.transform;
//        ar & elem_data.parent;
//        ar & elem_data.shape;
//        ar & elem_data.material;
//        ar & elem_data.curve;
//    }

    template<typename Archive>
    void serialize(Archive &ar, RegId &uid, const uint32_t version) {
        ar & to_integer(uid);
    }


}





BOOST_CLASS_VERSION(Pong::serializer::AssetSerializer, 0);


#endif //GL_TEST_SERIALIZER_H
