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
#include <array>

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

enum class TextureWrap : int32_t {
    REPEAT = 0,
    CLAMP_TO_EDGE = 1,
    MIRRORED_REPEAT = 2
};
void test_enum_type() {
    std::underlying_type<TextureWrap>::type{10};
}


class P {
private:
    std::string _resource{};
public:
    P(){ LOG_INFO("Constructor"); }
    P(const P& other) { LOG_INFO("Copy Constructor"); }
    P(P&& other) noexcept { LOG_INFO("Move Constructor"); }
};
#if false
P pass_val(P val) {
    LOG_INFO("Pass By rVal");
    return val;
}
#else
[[maybe_unused]] P pass_val(P&& val) {
    LOG_INFO("Pass By lval");
    return val;
}

P pass_val(const P& val) {
    LOG_INFO("Pass By const ref val");
    return val;
}
#endif

void test_value() {
    auto v0 = P();
    LOG_INFO("---");
    auto v2 = pass_val(v0);
    LOG_INFO("---");
    auto v3 = pass_val(std::move(v0));
    LOG_INFO("---");
    auto v4 = pass_val({});
}


int main() {
//    _test_text_template();
//    _test_shader_parser();
//    _test_sizes();

    test_value();


    return 0;

}