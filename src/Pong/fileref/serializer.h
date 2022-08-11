//
// Created by Jordi on 8/11/2022.
//

#ifndef GL_TEST_SERIALIZER_H
#define GL_TEST_SERIALIZER_H

#include <iostream>
#include <vector>
#include <boost/archive/text_iarchive.hpp>


namespace Pong::serializer {
    struct Header {
        uint32_t file_size;
        char file_type[10];
        uint8_t version;

        uint32_t num_elements;
        std::vector<uint32_t> elements_size;
    };

    struct Element {
        uint32_t data_size;
        char elem_type[10];
        uint32_t uid;
        char* data;
    };


    class MeshSerializer {

    };

    class MaterialSerializer {

    };

}

#endif //GL_TEST_SERIALIZER_H
