//
// Created by Jordi on 8/10/2022.
//

#ifndef GL_TEST_GEOFILE_H
#define GL_TEST_GEOFILE_H

#include "Pong/core/geometry_data.h"
#include "Pong/core/reg_id_manager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

namespace Pong {
    struct GeoElem {
        glm::mat4 transform;
        std::vector<Vertex> points;  // changes every frame

        std::vector<uint32_t> indices;

        RegId Material;
    };

    struct GroupElem {
        std::vector<GeoElem> group;
    };


    /**
     * Geo File loaded in memory with an uid assigned.
     * This class should be serializable into a file.
     * */
    struct GeoAsset {
    private:
        RegId uid;
        GroupElem meshes;

    public:


    };
}

#endif //GL_TEST_GEOFILE_H
