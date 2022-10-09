//
// Created by Jordi on 9/21/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_MAP_H_
#define PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_MAP_H_

#include "Pong/serializer/data_components.h"
#include "Pong/serializer/descriptor_base.h"
#include "Pong/map/map.h"


namespace pong::serializer {
// https://metricpanda.com/rival-fortress-update-39-how-i-use-__counter__-to-localize-text-and-hash-strings-at-compile-time/

    class MapDescriptor : public BaseDescriptor {
    public:

    public:
        FileHeader file_header{};
        ComponentData component_data{};
    };
    REG_DESCRIPTOR(MapDescriptor, 1);

    template<typename Archive>
    inline void serialize(Archive &ar, MapDescriptor &map_descriptor, const Version &version) {
        serialize(ar, map_descriptor.file_header, version);
        serialize(ar, map_descriptor.component_data, version);
    }


    struct MapDescriptorHelper {
        map::Map& map;
        MapDescriptor& descriptor;
    };

#define DESCRIPTOR_TO_MAP(n, DataM, k, total) \
    for(auto& head_dt: DataM.descriptor.component_data.COMPONENT_FIELD(n).data) { \
        DataM.map.entity_reg.insert_type<component::COMPONENT_CLASS(n)>(head_dt.header.reg_id, head_dt.data);  \
    }

    map::Map to_map(MapDescriptor & map_descriptor) {
        auto result = map::Map();
        result.reg_id = map_descriptor.file_header.reg_id;

        MapDescriptorHelper data = {result, map_descriptor};

        BOOST_PP_SEQ_FOR_EACH_I(DESCRIPTOR_TO_MAP, data, COMP_SEQ);

        return result;
    }


#define COMPONENT_TO_DESCRIPTOR(n, DataM, k, total) \
    auto& component_set_##n = DataM.map.entity_reg.get_types<component::COMPONENT_CLASS(n)>(); \
    auto reg_iter_##n = to_sparse_set_id_iter(component_set_##n); \
    auto & comp_dt_##n = DataM.descriptor.component_data.COMPONENT_FIELD(n); \
    for (auto& reg_id: reg_iter_##n) { \
        auto compo_##n = component_set_##n.at(reg_id); \
        compo_##n.actor = reg_id;                  \
        comp_dt_##n.data.push_back({{reg_id, 0}, std::move(compo_##n)}); \
    }


    MapDescriptor to_descriptor(map::Map & map) {
        auto result = MapDescriptor();
        result.file_header.reg_id = map.reg_id;
        MapDescriptorHelper data = {map, result};

        BOOST_PP_SEQ_FOR_EACH_I(COMPONENT_TO_DESCRIPTOR, data, COMP_SEQ);

        return result;
    }






}

#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_MAP_H_
