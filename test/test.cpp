//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "utils/text_template.h"
#include "Pong/logger.h"
#include <string>
#include <regex>
#include <fstream>
#include "utils/shader_parser.h"
#include "Pong/core/primitive_component.h"

//template<typename T>
std::string replace(const std::smatch& t) {
    return "adios";
}

void _test_text_template() {
    auto _string = std::string(R"({ pepe } { pepe["hola"]["que"]["pasa"] }  )");

    auto tmp = TextTemplate(_string);

    FormatValues val = {
            {"pepe", {"paco",
                      {{"hola", {"winchirl",
                                 {{"que", {"jeje",
                                           {{"pasa", {"aquirl!", {}}}
                                           }}
                                  }}
                      }}}
            }
            }
    };

    tmp.set_template_data(val);
    LOG_INFO(tmp.render());
}

void _test_shader_parser() {
    auto result = shaders_from_file(
            "./Shaders/raymarching_shaders/primitives_scene.glsl");

    for (auto& shdr: result) {
        LOG_INFO(shdr.second.get_data());
    }


}

void _test_sizes() {
    LOG_INFO("Position size " << sizeof(Pong::Vertex::position));
    LOG_INFO("OFFSET texcoords " << offsetof(Pong::Vertex, tex_coords));
}

int main() {
//    _test_text_template();
//    _test_shader_parser();
    _test_sizes();

    return 0;

}