//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIAL_DATA_H
#define GL_TEST_SERIAL_DATA_H

#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"
#include "Pong/core/material.h"
#include "Pong/file_data/serialization.h"
#include "Utils/type_conditions.h"

#include <concepts>
#include <iostream>
#include <ostream>
#include <vector>
#include <functional>
#include <optional>
#include <vector>
#include <string>
#include <cassert>


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

    class base_descriptor_ {};

    template<typename U>
    class Descriptor_: public base_descriptor_ {
    public:
        template<typename T>
        using vector_data = std::vector<T>;
    public:
        P_BASE_DESCRIPTION_DATA;
    };

    template<>
    class Descriptor_<ref_wrapper_type>: public base_descriptor_ {
    public:
        template<typename T>
        using vector_data = std::vector<std::reference_wrapper<T>>;
    public:
        P_BASE_DESCRIPTION_DATA;
    };

    using IAssetDescriptor = Descriptor_<any_type>;
    using OAssetDescriptor = Descriptor_<ref_wrapper_type>;
    REG_DESCRIPTOR(IAssetDescriptor, 1);
    REG_DESCRIPTOR(OAssetDescriptor, 1);


    template <Intersects<OAssetDescriptor, IAssetDescriptor> Descriptor>
    inline std::string ensure_file_name(Descriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(".asset")) {
            string_path.append(".asset");
        }

        return string_path;
    }

    template<typename Archive, Intersects<OAssetDescriptor, IAssetDescriptor> Descriptor>
    inline void serialize(Archive &ar, Descriptor &descriptor, const Version &version) {
        ar & descriptor.elem_data;
        ar & descriptor.mesh_data;
        ar & descriptor.curve_data;
        ar & descriptor.material_data;
    }

    template<std::derived_from<base_descriptor_> T>
    inline void save_file(
            T &descriptor,
            const char *file_name) {
        auto ofstream = std::ofstream(
                ensure_file_name(descriptor, file_name),
                std::ofstream::binary
        );

        auto srlizer = OSSerializer(ofstream);
        srlizer << descriptor;
    }

    template<std::derived_from<base_descriptor_> T>
    inline void load_file(
            T &descriptor,
            const char *file_name
    ) {

        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = ISSerializer(ifstream);
        srlizer >> descriptor;
    }

}

#endif //GL_TEST_SERIAL_DATA_H
