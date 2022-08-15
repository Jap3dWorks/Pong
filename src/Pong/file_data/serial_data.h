//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIAL_DATA_H
#define GL_TEST_SERIAL_DATA_H

#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include "Pong/file_data/serialization.h"

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
    IMPL_SERIALIZE(ElemData);

    struct MeshData {
        SERIALIZABLE(
                FIELD(RegId, uid),
                FIELD(Mesh, mesh)
        )
    };
    IMPL_SERIALIZE(MeshData);

    struct CurveData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Curve, curve)
        )
    };
    IMPL_SERIALIZE(CurveData);

    struct MaterialData {
        SERIALIZABLE (
                FIELD(RegId, uid),
                FIELD(Material, material)
        )
    };
    IMPL_SERIALIZE(MaterialData);


#define P_BASE_DESCRIPTION_DATA \
    vector_data<ElemData> elem_data; \
    vector_data<MeshData> mesh_data; \
    vector_data<CurveData> curve_data; \
    vector_data<MaterialData> material_data;

    struct any_type{};
    struct ref_wrapper_type{};

    template<typename U>
    class BaseDescriptor_ {
    public:
        template<typename T>
        using vector_data = std::vector<T>;
    public:
        P_BASE_DESCRIPTION_DATA;
    };

    template<>
    class BaseDescriptor_<ref_wrapper_type> {
    public:
        template<typename T>
        using vector_data = std::vector<std::reference_wrapper<T>>;

    public:
        P_BASE_DESCRIPTION_DATA;
    };

    using IAssetDescriptor = BaseDescriptor_<any_type>;
    using OAssetDescriptor = BaseDescriptor_<ref_wrapper_type>;
    SERIAL_CLASS_VERSION(IAssetDescriptor, 1);
    SERIAL_CLASS_VERSION(OAssetDescriptor, 1);

    // ensure output filenames template
    template<typename T>
    inline std::string ensure_file_name(const T&, const char* file_name);

    inline std::string ensure_file_name(
            const OAssetDescriptor&,
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
            const OAssetDescriptor& descriptor,
            const char* file_name) {

        auto os = std::ofstream(
                ensure_file_name(descriptor, file_name),
                std::ios::binary
        );

        auto srlizer = OSSerializer(os);
        srlizer << descriptor;
    }

    template<typename Archive>
    inline void serialize(Archive &ar, OAssetDescriptor &descriptor, const Version &file_version) {
        ar & descriptor.elem_data;
        ar & descriptor.mesh_data;
        ar & descriptor.curve_data;
        ar & descriptor.material_data;
    }
}





#endif //GL_TEST_SERIAL_DATA_H
