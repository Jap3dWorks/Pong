//
// Created by Jordi on 8/23/2022.
//

#ifndef PONG_TEST_FILEDATA_TESTS_H_
#define PONG_TEST_FILEDATA_TESTS_H_

#include "Pong/serializer/serialize_descriptors.h"

#include "Pong/components/component.h"
#include "Pong/map/map.h"

#include "Pong/serializer/descriptors.h"
#include "Pong/serializer/map_descriptor.h"
#include "Utils/logger.h"

#include <iostream>


struct ReflectData {
    REFLECTABLE(
    FIELD(uint32_t, attr_1, 0),
    FIELD(const char *, attr_2, "attr_2_value"),
    FIELD(uint32_t, attr_3, 2)
    );
};


void test_class_reflection() {
    auto rdata = ReflectData();
    print_fields(rdata);
}


void test_save_serialize_data() {
    auto description = pong::serializer::AssetDescriptor();
    LOG_INFO(pong::serializer::ensure_file_name(description, "Hola"));
    LOG_INFO(pong::serializer::ensure_file_name(description, "Hola.asset"));

    auto elem = pong::serializer::NodeData{{}, {}};
//    description.entity_data.push_back({elem});
    description.actor_data.data.push_back({{}, elem});

    pong::serializer::save_file(
            description,
            "D:/_docs/Pong/test/test_archive"
    );

    LOG_INFO("-File serialized-");
}


void test_save_serialize_plane() {
    std::vector<pong::Vertex> vertices = {
            {{-1,-1, 0}, {0,0,-1}, {0,0}},
            {{1, -1, 0}, {0,0,-1}, {0,1}},
            {{1, 1, 0}, {0,0,-1}, {1,1}},
            {{-1, 1, 0}, {0,0,-1}, {1,0}}
    };

    std::vector<uint32_t> indices =
            {0, 1, 3, 1, 2, 3};

    auto mesh_ = pong::Mesh{vertices, indices};

    auto odescriptor = pong::serializer::AssetDescriptor();
    odescriptor.mesh_data.data.push_back({{1, {}}, mesh_});

    pong::serializer::save_file(
            odescriptor,
            "./content/plane"
    );

    // read plane.asset
    auto idescriptor =  pong::serializer::AssetDescriptor();
    pong::serializer::load_file(
            idescriptor,
            "./content/plane");

    assert(!idescriptor.mesh_data.data.empty() && "Data is empty");

    for(auto& d: idescriptor.mesh_data.data) {
        LOG_INFO(d.header.reg_id);
    }

    LOG_INFO("-test save serialize Success-");
}


void test_load_id_from_file() {
    auto descriptor = pong::serializer::AssetDescriptor();

    pong::serializer::load_reg_id<pong::Mesh, decltype(descriptor)>(
            descriptor, {1}, "./content/plane"
            );

    for (auto& d: descriptor.mesh_data.data) {
        assert(d.header.reg_id.id == 1 && "Wrong Id Loaded!");
        LOG_INFO(d.header.reg_id);
    }
}


void test_serialize_map() {
    auto tmap = pong::map::Map();

    tmap.entity_reg.reg_type<pong::component::TransformComp>();
    tmap.entity_reg.insert_type<pong::component::TransformComp>({1}, {});

    auto map_descriptor = pong::serializer::to_descriptor(tmap);

    pong::serializer::save_file(map_descriptor, "./content/testmap");

}


void filedata_tests() {
    test_save_serialize_plane();
    test_serialize_map();
}


#endif //PONG_TEST_FILEDATA_TESTS_H_
