//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIAL_DATA_H
#define GL_TEST_SERIAL_DATA_H

#include "Pong/core/geometry_data.h"
#include "Pong/registers/reg_id.h"
#include "Pong/core/material.h"
#include "Pong/file_data/serialization.h"
#include "Utils/type_conditions.h"
#include "Pong/config/config.h"
#include "Pong/components/component.h"

#include <concepts>
#include <iostream>
#include <ostream>
#include <vector>
#include <functional>
#include <optional>
#include <vector>
#include <string>
#include <cassert>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>


namespace Pong::serializer {

    struct ActorData {
        SERIALIZABLE(
                FIELD(size_t, size),
                FIELD(std::optional<RegId>, uid), // serialized maps will need
                FIELD(std::optional<RegId>, parent),
                FIELD(std::optional<std::vector<TransformComponent>>, transform_component),
                FIELD(std::optional<std::vector<CameraComponent>>, camera_component),
                FIELD(std::optional<std::vector<StaticMeshComponent>>, staticmesh_component),
                FIELD(std::optional<std::vector<CubemapComponent>>, cubemap_component)
        )
    };
    IMPL_SERIALIZE(ActorData);

    struct MeshData {
        SERIALIZABLE(
                FIELD(size_t, size),
                FIELD(RegId, uid),
                FIELD(Mesh, mesh)
        )
    };
    IMPL_SERIALIZE(MeshData);

    struct CurveData {
        SERIALIZABLE (
                FIELD(size_t, size),
                FIELD(RegId, uid),
                FIELD(Curve, curve)
        )
    };
    IMPL_SERIALIZE(CurveData);

    struct MaterialData {
        SERIALIZABLE (
                FIELD(size_t, size),
                FIELD(RegId, uid),
                FIELD(Material, material)
        )
    };
    IMPL_SERIALIZE(MaterialData);


    // Descriptors (data as file)

    struct any_type{};
    struct ref_wrapper_type{};

    class base_descriptor_ {};

// Asset descriptor

#define P_DESCRIPTOR_VECTOR_DATA(vecdata) \
    public: \
    template<typename T> \
    using vector_data = std::vector<vecdata>;


#define P_ASSET_DESCRIPTION_DATA \
    vector_data<ActorData> actor_data; \
    vector_data<MeshData> mesh_data; \
    vector_data<CurveData> curve_data; \
    vector_data<MaterialData> material_data;


    template<typename U>
    class AssetDescriptor_ : public base_descriptor_ {
    P_DESCRIPTOR_VECTOR_DATA(T);
    public:
        P_ASSET_DESCRIPTION_DATA;
    };

    template<>
    class AssetDescriptor_<ref_wrapper_type> : public base_descriptor_ {
    P_DESCRIPTOR_VECTOR_DATA(std::reference_wrapper<T>);
    public:
        P_ASSET_DESCRIPTION_DATA;
    };

    using IAssetDescriptor = AssetDescriptor_<any_type>;
    using OAssetDescriptor = AssetDescriptor_<ref_wrapper_type>;
    REG_DESCRIPTOR(IAssetDescriptor, 1);
    REG_DESCRIPTOR(OAssetDescriptor, 1);

// Map descriptor
#define P_MAP_DESCRIPTION_DATA \
    vector_data<ActorData> actor_data;

    template<typename U>
    class MapDescriptor_: public base_descriptor_ {
    P_DESCRIPTOR_VECTOR_DATA(T);
    public:
        P_MAP_DESCRIPTION_DATA;
    };

    template<>
    class MapDescriptor_<ref_wrapper_type>: public base_descriptor_ {
    P_DESCRIPTOR_VECTOR_DATA(std::reference_wrapper<T>);
    public:
        P_MAP_DESCRIPTION_DATA;
    };

    using IMapDescriptor = MapDescriptor_<any_type>;
    using OMapDescriptor = MapDescriptor_<ref_wrapper_type>;
    REG_DESCRIPTOR(IMapDescriptor, 1);
    REG_DESCRIPTOR(OMapDescriptor, 1);


    template <Intersects<OAssetDescriptor, IAssetDescriptor> Descriptor>
    inline std::string ensure_file_name(Descriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(P_ASSET_EXTENSION)) {
            string_path.append(P_ASSET_EXTENSION);
        }

        return string_path;
    }

    template <Intersects<OMapDescriptor, IMapDescriptor> Descriptor>
    inline std::string ensure_file_name(Descriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(P_MAPS_EXTENSION)) {
            string_path.append(P_MAPS_EXTENSION);
        }
        return string_path;
    }

    template<typename Archive, Intersects<OAssetDescriptor, IAssetDescriptor> Descriptor>
    inline void serialize(Archive &ar, Descriptor &descriptor, const Version &version) {
        ar & descriptor.actor_data;
        ar & descriptor.mesh_data;
        ar & descriptor.curve_data;
        ar & descriptor.material_data;
    }

    template<typename Archive, Intersects<OMapDescriptor, IMapDescriptor> Descriptor>
    inline void serialize(Archive &ar, Descriptor &descriptor, const Version &version) {
        ar & descriptor.actor_data;
    }

    template<std::derived_from<base_descriptor_> T>
    inline void save_file(T &descriptor, const char *file_name) {

        auto ofstream = std::ofstream(
                ensure_file_name(descriptor, file_name),
                std::ofstream::binary
        );

        auto srlizer = OSSerializer(ofstream);

        srlizer << descriptor;
    }

    template<std::derived_from<base_descriptor_> T>
    inline void load_file(T &descriptor, const char *file_name) {

        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = ISSerializer(ifstream);
        srlizer >> descriptor;
    }


    inline void set_sizes(OAssetDescriptor& descriptor) {
        auto size_desc = SizeSerializer();
        for (auto& dt: descriptor.actor_data) {
            serialize(size_desc, dt, {});
            dt.get().size = size_desc.get();
            size_desc.clear();
        }

        for (auto& dt: descriptor.mesh_data) {
            serialize(size_desc, dt, {});
            dt.get().size = size_desc.get();
            size_desc.clear();
        }

        for (auto& dt: descriptor.curve_data) {
            serialize(size_desc, dt, {});
            dt.get().size = size_desc.get();
            size_desc.clear();
        }

        for (auto& dt: descriptor.material_data) {
            serialize(size_desc, dt, {});
            dt.get().size = size_desc.get();
            size_desc.clear();
        }
    }
}

#endif //GL_TEST_SERIAL_DATA_H
