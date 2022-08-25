//
// Created by Jordi on 8/11/2022.
//

#ifndef PONG_SERIAL_DATA_H_
#define PONG_SERIAL_DATA_H_

#include "Pong/core/geometry_data.h"
#include "Pong/registers/reg_id.h"
#include "Pong/core/material.h"
#include "Pong/file_data/serializers.h"
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
#include <concepts>


namespace pong::serializer {

    struct ActorData {
        SERIALIZABLE(
                FIELD(RegId, parent, 0),
                FIELD(std::vector<TransformComponent>, transform_component),
                FIELD(std::vector<CameraComponent>, camera_component),
                FIELD(std::vector<StaticMeshComponent>, staticmesh_component),
                FIELD(std::vector<CubemapComponent>, cubemap_component)
        )
    };
    IMPL_SERIALIZE(ActorData);

    class BaseDescriptor {
    public:
        template<typename T>
        using HeadedDataT = HeadedData<Header<T>, T>;
        template<typename T>
        using SerializeDataT = HeadedDataT<std::vector<HeadedDataT<T>>>;
    };

    struct AssetData {
        SERIALIZABLE (
                FIELD(BaseDescriptor::SerializeDataT<ActorData>, actor_data),
                FIELD(BaseDescriptor::SerializeDataT<Mesh>, mesh_data),
                FIELD(BaseDescriptor::SerializeDataT<Curve>, curve_data),
                FIELD(BaseDescriptor::SerializeDataT<Material>, material_data)
        )
    };
    IMPL_SERIALIZE(AssetData);

    template<typename U>
    class AssetDescriptorT : public BaseDescriptor {
    public:
        HeadedData<FileHeader, AssetData> data{};
    public:
        SerializeDataT<ActorData>& actor_data{data.data.actor_data};
        SerializeDataT<Mesh>& mesh_data{data.data.mesh_data};
        SerializeDataT<Curve>& curve_data{data.data.curve_data};
        SerializeDataT<Material>& material_data{data.data.material_data};
    };


    using AssetDescriptor = AssetDescriptorT<Any_t>;
    REG_DESCRIPTOR(AssetDescriptor, 1);


    struct MapData {
        SERIALIZABLE (
                FIELD(BaseDescriptor::SerializeDataT<ActorData>, actor_data)
        )
    };
    IMPL_SERIALIZE(MapData);

    template<typename U>
    class MapDescriptorT: public BaseDescriptor {

    public:
        HeadedData<FileHeader, MapData> data{};
        SerializeDataT<ActorData>& actor_data{data.data.actor_data};
    };


    using MapDescriptor = MapDescriptorT<Any_t>;
    REG_DESCRIPTOR(MapDescriptor, 1);


//    template <Intersects<AssetDescriptor, AssetDescriptor> Descriptor>
    inline std::string ensure_file_name(AssetDescriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(P_ASSET_EXTENSION)) {
            string_path.append(P_ASSET_EXTENSION);
        }

        return string_path;
    }

    inline std::string ensure_file_name(MapDescriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(P_MAPS_EXTENSION)) {
            string_path.append(P_MAPS_EXTENSION);
        }
        return string_path;
    }

    template<typename Archive, std::derived_from<BaseDescriptor> Descriptor>
    inline void serialize(Archive &ar, Descriptor &descriptor, const Version &version) {
        ar & descriptor.data;
    }

    template<std::derived_from<BaseDescriptor> Descriptor>
    inline void save_file(Descriptor &descriptor, const char *file_name) {

        auto ofstream = std::ofstream(
                ensure_file_name(descriptor, file_name),
                std::ofstream::binary
        );

        auto size_ser = SizeSerializer();
        size_ser >> descriptor;

        auto out_ser = OSSerializer(ofstream);

        out_ser << descriptor;
    }

    template<std::derived_from<BaseDescriptor> Descriptor>
    inline void load_file(Descriptor &descriptor, const char *file_name) {

        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = ISSerializer(ifstream);
        srlizer >> descriptor;
    }

    template<std::derived_from<BaseDescriptor> Descriptor>
    inline void load_headers(Descriptor & descriptor, const char *file_name) {
        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = ISHeaderSerializer(ifstream);
        srlizer >> descriptor;
    }

    template<typename data_type, std::derived_from<BaseDescriptor> Descriptor>
    inline void load_reg_id(Descriptor & descriptor, RegId reg_id, const char *file_name) {
        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = IRegIdSerializer<data_type>(ifstream, reg_id);

        srlizer >> descriptor;
    }

}

#endif //PONG_SERIAL_DATA_H_
