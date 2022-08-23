//
// Created by Jordi on 8/23/2022.
//

#ifndef PONG_FILEDATA_TESTS_H
#define PONG_FILEDATA_TESTS_H

#include "Pong/file_data/reflectable.h"
#include "Pong/file_data/serial_data.h"
#include "Pong/file_data/serialization.h"
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
    auto description = Pong::serializer::OAssetDescriptor();
    LOG_INFO(Pong::serializer::ensure_file_name(description, "Hola"));
    LOG_INFO(Pong::serializer::ensure_file_name(description, "Hola.asset"));

    auto elem = Pong::serializer::ActorData{{}, {}};
//    description.actor_data.push_back({elem});
    description.actor_data.data.push_back({{}, elem});

    Pong::serializer::save_file(
            description,
            "D:/_docs/Pong/test/test_archive"
    );

    LOG_INFO("-File serialized-");
}


void test_save_serialize_plane() {
    std::vector<Pong::Vertex> vertices = {
            {{-1,-1, 0}, {0,0,-1}, {0,0}},
            {{1, -1, 0}, {0,0,-1}, {0,1}},
            {{1, 1, 0}, {0,0,-1}, {1,1}},
            {{-1, 1, 0}, {0,0,-1}, {1,0}}
    };

    std::vector<uint32_t> indices =
            {0, 1, 3, 1, 2, 3};

    auto mesh_ = Pong::Mesh{vertices, indices};


    auto odescriptor = Pong::serializer::OAssetDescriptor();
    odescriptor.mesh_data.data.push_back({{1, {}}, mesh_});

    Pong::serializer::save_file(
            odescriptor,
            "./assets/plane"
    );

    // read plane.asset
    auto idescriptor =  Pong::serializer::IAssetDescriptor();
    Pong::serializer::load_file(
            idescriptor,
            "./assets/plane");

    assert(!idescriptor.mesh_data.data.empty() && "Data is empty");

    for(auto& d: idescriptor.mesh_data.data) {
        LOG_INFO(d.header.reg_id);
    }

    LOG_INFO("-test save serialize Success-");

}






#endif //PONG_FILEDATA_TESTS_H
