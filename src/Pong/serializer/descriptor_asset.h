//
// Created by Jordi on 8/11/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_ASSET_H_
#define PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_ASSET_H_

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

#include "Pong/serializer/descriptor_base.h"
#include "Pong/core/geometry_data.h"
#include "Pong/registers/reg_id.h"
#include "Pong/core/material.h"
#include "Pong/serializer/serializers.h"
#include "Utils/type_conditions.h"
#include "Pong/config/config.h"
#include "Pong/components/component.h"
#include "Pong/map/map.h"
#include "Pong/serializer/data_base.h"
#include "Pong/serializer/data_components.h"



namespace pong::serializer {
    struct ShapeData {
    SERIALIZABLE (
                FIELD(SerializeDataT<Mesh>, mesh_data),
                FIELD(SerializeDataT<Curve>, curve_data),
                FIELD(SerializeDataT<Material>, material_data)
        )
    };
    IMPL_SERIALIZE(ShapeData);

    IMPL_GET_DATA_FLD(ShapeData, Mesh, mesh_data);
    IMPL_GET_DATA_FLD(ShapeData, Curve, curve_data);
    IMPL_GET_DATA_FLD(ShapeData, Material, material_data);


    class AssetDescriptor : public BaseDescriptor {
    public:
        FileHeader file_header{};
        ComponentData component_data{};
        ShapeData shape_data{};
    };
    REG_DESCRIPTOR(AssetDescriptor, 1);

    template<typename Archive>
    inline void serialize(Archive &ar, AssetDescriptor &asset_descriptor, const Version &version) {
        serialize(ar, asset_descriptor.file_header, version);
        serialize(ar, asset_descriptor.component_data, version);
        sersialize(ar, asset_descriptor.shape_data, version);
    }

}

#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_ASSET_H_
