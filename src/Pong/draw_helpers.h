//
// Created by Jordi on 4/19/2022.
//

#ifndef PONG_DRAW_HELPERS_H
#define PONG_DRAW_HELPERS_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <vector>

#include "Pong/core/actor/actor.h"
#include "Pong/core/map/map.h"
#include "Pong/components/graphic_component.h"
#include "Pong/core/shader.h"
#include "Pong/core/material.h"


namespace Pong {
    void draw_point(glm::vec3 point)
    {
        Map* scene = Map::get_instance();

        // point Actor name
        std::string pnt_nme = "point_";
        int number = 0;
        while (true)
        {
            std::string try_nme = pnt_nme + std::to_string(number);
            if (scene->actor_map.find(try_nme) == scene->actor_map.end())
            {
                pnt_nme = try_nme;
                break;
            }
            else
            {
                number++;
            }
        }

        // get shader
        Shader* point_shd = scene->create_shader(
                "point_shd",
                "unlit.glsl");

        auto point_mat = scene->create_material<MaterialController>("point_mat", point_shd,
                                                                    std::vector<Pong::Texture *>());

        // point Actor
        auto* a_point = scene->create_actor<AKinetic>(pnt_nme);
        auto* s_point = scene->create_shape<IcosphereShape>("point_shp");
        a_point->translation = glm::translate(glm::mat4(1), point);
        s_point->set_radius(0.05f);
        scene->assign_material(point_mat, s_point);
        scene->assign_shape(s_point, a_point);

        LOG_DEBUG("Point created-> " << pnt_nme << " at-> " <<
                                     point.x << ", " << point.y << ", " << point.z);
    }
}
#endif //PONG_DRAW_HELPERS_H
