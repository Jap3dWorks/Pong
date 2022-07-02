//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <vector>
#include <iostream>
#include <array>
#include <fstream>
#include <set>

#include "utils/text_template.h"
#include "utils/shader_parser.h"
#include "utils/subclasses_map.h"
#include "Pong/logger.h"
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

template<typename T>
struct OrderComparer;

template<typename C>
struct OrderComparer<std::pair<uint32_t, C>>{
    bool operator()(const std::pair<uint32_t, C>& left,
            const std::pair<uint32_t, C>& right) const
    {
        return left.first < right.first;
    }
};

using matComparer = const OrderComparer<std::pair<uint32_t, std::string>>;

void test_multiset() {
    std::map<uint32_t,
            std::multiset<std::pair<uint32_t, std::string>, matComparer>>
            maptest;

    maptest[0].insert(
            {1, std::string("abc")});

}

struct TClass {
    int a=0;
    TClass() = default;
    explicit TClass(int _v): a(_v) {}
};

struct SClassA: public TClass {
    SClassA(): TClass(1){}
};
struct SClassB: public TClass {
    SClassB(): TClass(2){}
};
struct SClassC: public TClass {
    SClassC(): TClass(3){}
};

void test_mapIteration() {
    auto sclassmap = SubClassMap<TClass>();

    sclassmap.register_type<SClassA>();
    sclassmap.register_type<SClassB>();
    sclassmap.register_type<SClassC>();

    auto a = SClassA();
    auto b = SClassB();
    auto c = SClassC();

    sclassmap.push_back<SClassA>(&a);
    sclassmap.push_back<SClassB>(&b);
    sclassmap.push_back<SClassC>(&c);

    for (auto& it : sclassmap) {
        LOG_INFO(it->a);
    }

    LOG_INFO("-SClassB values-");
    auto itr = sclassmap.begin<SClassB>();
    auto end = sclassmap.end<SClassB>();
    for(;itr!=end; ++itr) {
        LOG_INFO((*itr)->a);
    }
}

void test_vectorit() {
    auto a = SClassA();
    auto b = SClassB();
    auto c = SClassC();

    auto _v = std::vector<TClass*>{&a, &b, &c};
    auto& _v2 = _v;
//    _v2.data
    auto begin = _v.begin();
    auto end = _v2.end();

    if (begin != end) {
        LOG_INFO("Vector Not Equal");
    }
    else {
        LOG_INFO("Vector Equal");
    }
}

int main() {
//    _test_text_template();
//    _test_shader_parser();
//    _test_sizes();
//    test_value();
//    test_multiset();

//    test_vectorit();
    test_mapIteration();

    return 0;

}