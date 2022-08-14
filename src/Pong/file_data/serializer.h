//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIALIZER_H
#define GL_TEST_SERIALIZER_H

#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include "Pong/file_data/reflectable.h"
#include "Pong/file_data/serialization.h"

#include <boost/serialization/string.hpp>

#include <iostream>
#include <ostream>
#include <vector>
#include <functional>
#include <optional>
#include <vector>
#include <string>


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
    SERIALIZABLE_IMPL(ElemData);

    struct MeshData {
        SERIALIZABLE(
                FIELD(RegId, uid),
                FIELD(Mesh, mesh)
        )
    };
    SERIALIZABLE_IMPL(MeshData);

    struct CurveData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Curve, curve)
        )
    };
    SERIALIZABLE_IMPL(CurveData);

    struct MaterialData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Material, material)
        )
    };
    SERIALIZABLE_IMPL(MaterialData);


#define P_BASE_DESCRIPTION_DATA \
    vector_data<ElemData> elem_data; \
    vector_data<MeshData> mesh_data; \
    vector_data<CurveData> curve_data; \
    vector_data<MaterialData> material_data;

    struct any_type{};
    struct ref_wrapper_type{};

    template<typename U>
    class BaseDescription_ {
    public:
        template<typename T>
        using vector_data = std::vector<T>;
    public:
        P_BASE_DESCRIPTION_DATA;
    };

    template<>
    class BaseDescription_<ref_wrapper_type> {
    public:
        template<typename T>
        using vector_data = std::vector<std::reference_wrapper<T>>;

    public:
        P_BASE_DESCRIPTION_DATA;
    };

    using IAssetDescription = BaseDescription_<any_type>;
    using OAssetDescription = BaseDescription_<ref_wrapper_type>;


    // ensure output filenames template
    template<typename T>
    std::string ensure_file_name(const T&, const char* file_name);

    std::string ensure_file_name(
            const OAssetDescription&,
            const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(".asset")) {
            string_path.append(".asset");
        }

        return string_path;
    }


    // Save serialized
    template <typename T>
    void save_file(const T&,  const char*);


    void save_file(
            const OAssetDescription& asset_serializer,
            const char* file_name) {

        auto os = std::ofstream(
                ensure_file_name(asset_serializer, file_name).c_str(),
                std::ios::binary
        );

        const char *test = "test";
        auto number = 10;

        auto srlizer = OSSerializer(os);
        srlizer << number;

    }


    template<typename Archive>
    void serialize(Archive &ar, RegId &uid, const uint32_t version) {
        ar & to_integer(uid);
    }

    SERIAL_CLASS_VERSION(OAssetDescription, 1);
    SERIAL_CLASS_VERSION(IAssetDescription, 1);

}





#endif //GL_TEST_SERIALIZER_H
