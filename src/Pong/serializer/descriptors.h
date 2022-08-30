//
// Created by Jordi on 8/11/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DESCRIPTORS_H_
#define PONG_SRC_PONG_SERIALIZER_DESCRIPTORS_H_

#include "Pong/core/geometry_data.h"
#include "Pong/registers/reg_id.h"
#include "Pong/core/material.h"
#include "Pong/serializer/serializers.h"
#include "Utils/type_conditions.h"
#include "Pong/config/config.h"
#include "Pong/components/component.h"
#include "Pong/map/map.h"

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
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <unordered_map>


namespace pong::serializer {

    struct EntityData {
        SERIALIZABLE(
                FIELD(RegId, parent, 0),
                FIELD(component::TransformComponent, transform_component),
                FIELD(component::CameraComponent, camera_component),
                FIELD(component::StaticMeshComponent, staticmesh_component),
                FIELD(component::CubemapComponent, cubemap_component)
        )
    };
    IMPL_SERIALIZE(EntityData);

    class BaseDescriptor {
    public:
        template<typename T>
        using HeadedDataT = HeadedData<Header<T>, T>;
        template<typename T>
        using SerializeDataT = HeadedDataT<std::vector<HeadedDataT<T>>>;
    };

    struct AssetData {
        SERIALIZABLE (
                FIELD(BaseDescriptor::SerializeDataT<EntityData>, actor_data),
                FIELD(BaseDescriptor::SerializeDataT<Mesh>, mesh_data),
                FIELD(BaseDescriptor::SerializeDataT<Curve>, curve_data),
                FIELD(BaseDescriptor::SerializeDataT<Material>, material_data)
        )
    };
    IMPL_SERIALIZE(AssetData);

    template<typename U>
    class AssetDescriptorT : public BaseDescriptor {
    public:
        using DataType = HeadedData<FileHeader, AssetData>;
    public:
        DataType data{};
    public:
        SerializeDataT<EntityData>& actor_data{data.data.actor_data};
        SerializeDataT<Mesh>& mesh_data{data.data.mesh_data};
        SerializeDataT<Curve>& curve_data{data.data.curve_data};
        SerializeDataT<Material>& material_data{data.data.material_data};
    };


    using AssetDescriptor = AssetDescriptorT<Any_t>;
    REG_DESCRIPTOR(AssetDescriptor, 1);


    struct MapData {
        SERIALIZABLE (
                FIELD(BaseDescriptor::SerializeDataT<EntityData>, entity_data)
        )
    };
    IMPL_SERIALIZE(MapData);

    template<typename U>
    class MapDescriptorT: public BaseDescriptor {

    public:
        using DataType = HeadedData<FileHeader, HeadedDataT<MapData>>;
    public:
        DataType data{};
        SerializeDataT<EntityData>& entity_data{data.data.data.entity_data};
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

        auto out_ser = OSerializer(ofstream);

        out_ser << descriptor;
    }

    template<std::derived_from<BaseDescriptor> Descriptor>
    inline void load_file(Descriptor &descriptor, const char *file_name) {

        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = ISerializer(ifstream);
        srlizer >> descriptor;
    }

    template<std::derived_from<BaseDescriptor> Descriptor>
    inline void load_headers(Descriptor & descriptor, const char *file_name) {
        auto ifstream = std::ifstream(
                ensure_file_name(descriptor, file_name),
                std::ifstream::binary);

        assert(ifstream.is_open() && "File doesn't exists!");

        auto srlizer = IHeaderSerializer(ifstream);
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

    template<typename Descriptor, typename DtType>
    struct descriptor_data {
    };

#define IMPL_DESCRIPTOR_DATA(asset_descriptor_type, data_type, data_member) \
    template<> \
    struct descriptor_data<asset_descriptor_type, data_type> { \
    using DescriptorType = asset_descriptor_type; \
    using DataType = data_type; \
    const DescriptorType& descriptor; \
    explicit descriptor_data(const DescriptorType &descriptor_p) : \
        descriptor(descriptor_p) { \
    } \
    const auto cbegin() { \
        return descriptor.data_member.data.cbegin(); \
    } \
    const auto cend() { \
        return descriptor.data_member.data.cend(); \
    } \
    const auto begin() { \
        return descriptor.data_member.data.begin(); \
    } \
    const auto end() { \
        return descriptor.data_member.data.end(); \
    } \
    }

    IMPL_DESCRIPTOR_DATA(AssetDescriptor, EntityData, actor_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Mesh, mesh_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Curve, curve_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Material, material_data);

    IMPL_DESCRIPTOR_DATA(MapDescriptor, MapData, entity_data);


    MapDescriptor translate_map(map::Map& map) {
        auto result = MapDescriptor();
        result.data.data.header.reg_id = map.reg_id;

        auto temp_dt = std::unordered_map<RegId, component::TransformComponent>;



        using Range = boost::mpl::range_c<uint32_t, 0, map::EntityComponentsTypes::count>;
        boost::mpl::for_each<Range>(
            [&]<typename I>(I i) constexpr -> void {
//                data_visitor<MapDescriptor, map::Map, TranslateVisitor, I>(
//                    result, map.map_register.component_reg, TranslateVisitor(), i
//                    );

                auto& sparse_set = map.map_register.component_reg.template get_types<
                    typename map::EntityComponentsTypes::get<I::value>::type>();

                for(auto& reg_id: SparseSetRegIdIter(sparse_set)) {

                    sparse_set.at(reg_id);
                }

             }
        );

//        map.map_register.component_reg.get_types<>()

    }

}

#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTORS_H_
