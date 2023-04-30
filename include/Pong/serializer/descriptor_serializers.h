//
// Created by Jordi on 10/3/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_SERIALIZERS_H_
#define PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_SERIALIZERS_H_

#include "Pong/serializer/reflectable.h"
#include "Pong/components/component.h"
#include "Pong/map/map.h"

#include "Pong/serializer/descriptor_asset.h"
#include "Pong/serializer/descriptor_map.h"

namespace pong::serializer {

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



}


#endif //PONG_SRC_PONG_SERIALIZER_DESCRIPTOR_SERIALIZERS_H_
