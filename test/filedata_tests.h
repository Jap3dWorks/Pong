//
// Created by Jordi on 8/23/2022.
//

#ifndef PONG_FILEDATA_TESTS_H
#define PONG_FILEDATA_TESTS_H

#include "Pong/serial_data/reflectable.h"
#include "Pong/serial_data/header_data.h"
#include "Pong/serial_data/serializers.h"
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

    auto elem = pong::serializer::ActorData{{}, {}};
//    description.actor_data.push_back({elem});
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
            "./assets/plane"
    );

    // read plane.asset
    auto idescriptor =  pong::serializer::AssetDescriptor();
    pong::serializer::load_file(
            idescriptor,
            "./assets/plane");

    assert(!idescriptor.mesh_data.data.empty() && "Data is empty");

    for(auto& d: idescriptor.mesh_data.data) {
        LOG_INFO(d.header.reg_id);
    }

    LOG_INFO("-test save serialize Success-");
}


void test_load_id_from_file() {
    auto descriptor = pong::serializer::AssetDescriptor();

    pong::serializer::load_reg_id<pong::Mesh, decltype(descriptor)>(
            descriptor, {1}, "./assets/plane"
            );

    for (auto& d: descriptor.mesh_data.data) {
        assert(d.header.reg_id.id == 1 && "Wrong Id Loaded!");
        LOG_INFO(d.header.reg_id);
    }
}







#endif //PONG_FILEDATA_TESTS_H
