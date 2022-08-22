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


    struct oasset_t{};

    class base_descriptor_ {
    public:
        template<typename T>
        using headed_data_t = HeadedData<Header<T>, T>;
        template<typename T>
        using serialize_data_t = headed_data_t<std::vector<headed_data_t<T>>>;
    };

    template<typename U>
    class AssetDescriptor_ : public base_descriptor_ {
    public:
        // file header


        serialize_data_t<ActorData> actor_data;
        serialize_data_t<MeshData> mesh_data;
        serialize_data_t<CurveData> curve_data;
        serialize_data_t<MaterialData> material_data;
    };


    using IAssetDescriptor = AssetDescriptor_<Any_t>;
    using OAssetDescriptor = AssetDescriptor_<oasset_t>;
    REG_DESCRIPTOR(IAssetDescriptor, 1);
    REG_DESCRIPTOR(OAssetDescriptor, 1);


    template<typename U>
    class MapDescriptor_: public base_descriptor_ {

    public:
        serialize_data_t<ActorData> actor_data;
    };


    using IMapDescriptor = MapDescriptor_<Any_t>;
    using OMapDescriptor = MapDescriptor_<oasset_t>;
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

        auto sizeser = SizeSerializer();
        sizeser >> descriptor;

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

}

#endif //GL_TEST_SERIAL_DATA_H
