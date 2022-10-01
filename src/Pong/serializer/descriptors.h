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

    struct NodeData {
        SERIALIZABLE(
                FIELD(RegId, parent, 0),
                // TODO: reflection registered components.
                FIELD(std::optional<component::TransformComp>, transform_component),
                FIELD(std::optional<component::CameraComp>, camera_component),
                FIELD(std::optional<component::StaticMeshComp>, staticmesh_component),
                FIELD(std::optional<component::CubemapComp>, cubemap_component),
                FIELD(std::optional<component::PythonComp>, python_component)
        )
    };
    IMPL_SERIALIZE(NodeData);

    template<typename T>
    using HeadedDataT = HeadedData<Header<T>, T>;

    template<typename T>
    using SerializeDataT = HeadedDataT<std::vector<HeadedDataT<T>>>;

    class BaseDescriptor {};

    struct AssetData {
        SERIALIZABLE (
                FIELD(SerializeDataT<NodeData>, actor_data),
                FIELD(SerializeDataT<Mesh>, mesh_data),
                FIELD(SerializeDataT<Curve>, curve_data),
                FIELD(SerializeDataT<Material>, material_data)
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
        SerializeDataT<NodeData>& actor_data{data.data.actor_data};
        SerializeDataT<Mesh>& mesh_data{data.data.mesh_data};
        SerializeDataT<Curve>& curve_data{data.data.curve_data};
        SerializeDataT<Material>& material_data{data.data.material_data};
    };


    using AssetDescriptor = AssetDescriptorT<Any_t>;
    REG_DESCRIPTOR(AssetDescriptor, 1);


    struct MapData {
        SERIALIZABLE (
                FIELD(std::optional<component::PythonComp>, python_component),
                FIELD(SerializeDataT<NodeData>, entity_data)
        )
    };
    IMPL_SERIALIZE(MapData);


//    template <Intersects<AssetDescriptor, AssetDescriptor> Descriptor>
    inline std::string ensure_file_name(AssetDescriptor&,  const char* file_name) {

        auto string_path = std::string(file_name);
        if (!string_path.ends_with(P_ASSET_EXTENSION)) {
            string_path.append(P_ASSET_EXTENSION);
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

    IMPL_DESCRIPTOR_DATA(AssetDescriptor, NodeData, actor_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Mesh, mesh_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Curve, curve_data);
    IMPL_DESCRIPTOR_DATA(AssetDescriptor, Material, material_data);

//    IMPL_DESCRIPTOR_DATA(MapDescriptor, NodeData, entity_data);


#define IMPL_ADD_COMPONENT(data_type, component_type, member) \
    void add_component(data_type &data, const component_type &component) { \
    data.member = component;\
}

    IMPL_ADD_COMPONENT(NodeData, component::TransformComp, transform_component)
    IMPL_ADD_COMPONENT(NodeData, component::CameraComp, camera_component)
    IMPL_ADD_COMPONENT(NodeData, component::StaticMeshComp, staticmesh_component)
    IMPL_ADD_COMPONENT(NodeData, component::CubemapComp, cubemap_component)
    IMPL_ADD_COMPONENT(NodeData, component::PythonComp, python_component)


    template<typename T, typename U>
    struct get_component;

#define IMPL_GET_COMPONENT(data_type, component_type, member) \
    template<> \
    struct get_component<data_type, component_type> {         \
        using DataType = data_type;\
        using ComponentType = component_type; \
        static inline std::optional<ComponentType> get(const DataType &data_type) { \
            return data_type.member; \
        } \
    };

    IMPL_GET_COMPONENT(NodeData, component::TransformComp, transform_component)
    IMPL_GET_COMPONENT(NodeData, component::CameraComp, camera_component)
    IMPL_GET_COMPONENT(NodeData, component::PythonComp, python_component)
    IMPL_GET_COMPONENT(NodeData, component::StaticMeshComp, staticmesh_component)
    IMPL_GET_COMPONENT(NodeData, component::CubemapComp, cubemap_component)


//    MapDescriptor to_descriptor(map::Map &map) {
//        auto result = MapDescriptor();
//        result.data.data.header.reg_id = map.reg_id;
//
//        auto temp_dt = std::unordered_map<RegId, NodeData>();
//
//        using Range = boost::mpl::range_c<uint32_t, 0, map::EntityComponentsTypes::count>;
//        boost::mpl::for_each<Range>(
//            [&]<typename I>(I i) constexpr -> void {
//                auto& sparse_set = map.entity_reg.template get_types<
//                    typename map::EntityComponentsTypes::get<I::value>::type>();
//
//                for(auto& reg_id: SparseSetRegIdIter(sparse_set)) {
//                    auto value = sparse_set.at(reg_id);
//                    if (!temp_dt.contains(reg_id)) {
//                        temp_dt[reg_id] = {};
//                    }
//                    add_component(temp_dt[reg_id], value);
//                }
//             }
//        );
//
//        for (auto &p : temp_dt) {
//            result.entity_data.data.push_back(
//                {{p.first}, p.second}
//            );
//        }
//
//        return result;
//
//    }

//    map::Map from_descriptor(MapDescriptor & descriptor) {
//        auto result = map::Map();
//        result.reg_id = descriptor.map_data.header.reg_id;
//        using Range = boost::mpl::range_c<uint32_t, 0, map::EntityComponentsTypes::count>;
//
//        for (auto& dt: descriptor_data<MapDescriptor, NodeData>(descriptor)) {
//            auto reg_id = dt.header.reg_id;
//
//            auto entity = dt.data;
//
//            boost::mpl::for_each<Range>(
//                [&]<typename I>(I i) constexpr -> void {
//                    auto component = get_component<
//                        NodeData,
//                        typename map::EntityComponentsTypes::get<I::value>::type
//                    >::get(entity);
//
//                    if (component) {
//                        result.entity_reg.template insert_type<
//                            typename decltype(component)::value_type>(
//                            reg_id, component.value()
//                        );
//                    }
//                }
//            );
//        }
//
//        if (descriptor.map_data.data.python_component) {
//            result.python_component = descriptor.map_data.data.python_component.value();
//        }
//
//        return result;
//    }



}

#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTORS_H_
