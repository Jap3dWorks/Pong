//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "utils/text_template.h"
#include "Pong/logger.h"
#include <string>
#include <regex>
#include <fstream>
#include "Pong/utils/shader_parser.h"

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
    auto result = shaders_from_file("./Shaders/raymarching_shaders/voronoi_scene.glsl");

    for (auto& shdr: result) {
        LOG_INFO(shdr.second.get_data());
    }


}

int main() {
//    _test_text_template();
    _test_shader_parser();

    return 0;

}