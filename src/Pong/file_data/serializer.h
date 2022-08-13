//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIALIZER_H
#define GL_TEST_SERIALIZER_H


#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include "Pong/file_data/reflectable.h"

//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>

#include <iostream>
#include <ostream>
#include <vector>
#include <functional>
#include <optional>
#include <vector>
#include <string>

#define SERIALIZABLE REFLECTABLE

namespace Pong::serializer {
    struct ElemData {
        SERIALIZABLE(
                FIELD(glm::mat4, transform, 1),
                FIELD(std::optional<RegId>, parent),
                FIELD(std::optional<RegId>, shape),
                FIELD(std::optional<RegId>, material),
                FIELD(std::optional<RegId>, curve)
        )
    };

    struct MeshData {
        SERIALIZABLE(
                FIELD(RegId, uid),
                FIELD(Mesh, mesh)
        )
    };

    struct CurveData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Curve, curve)
        )
    };

    struct MaterialData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Material, material)
        )
    };


    // ensure output filenames template
    template<typename T>
    std::string ensure_file_name(const T&, const char* file_name);

    class AssetSerializer;
    template<>
    std::string ensure_file_name<AssetSerializer>(
            const AssetSerializer&,
            const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(".asset")) {
            string_path.append(".asset");
        }

        return string_path;
    }


    // Save serialized
    template <typename T>
    void save_serialized(const T&,  const char*);


// Try to use reflection for struct serialization.
// https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application
    class AssetSerializer {
        friend class boost::serialization::access;

    public:
        template<typename T>
//        using list_reference =  std::vector<std::reference_wrapper<T>>;
        using list_reference =  std::vector<T>;

    public:
        list_reference<ElemData> elem_data;
        list_reference<MeshData> mesh_data;
        list_reference<CurveData> curve_data;
        list_reference<MaterialData> material_data;

        template<typename Archive>
        void save(Archive & ar, const uint32_t version) const {
//            ar & elem_data;
//            ar & mesh_data;
//            ar & curve_data;
//            ar & material_data;
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


    template<>
    void save_serialized<AssetSerializer>(
            const AssetSerializer& asset_serializer, const char* file_name) {

        {
            auto ofs = std::ofstream(
                    ensure_file_name(asset_serializer, file_name).c_str(),
                    ios::binary);

            const char* test = "test";
            auto number = 10;

//            ofs.write((char*)test, 5);
//            ofs.write((char*)&number, sizeof(number));

//            auto out = boost::archive::binary_oarchive(ofs);
//            out << number;

        }


    }


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


BOOST_CLASS_VERSION(Pong::serializer::AssetSerializer, 0);


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive &ar, RegId &uid, const uint32_t version) {
        ar & to_integer(uid);
    }

    template<typename Archive, typename SubType>
    void serialize(Archive &ar, std::reference_wrapper<SubType> &uid, const uint32_t version) {
        ar & &uid;
    }

}


#endif //GL_TEST_SERIALIZER_H
