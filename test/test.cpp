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
#include "utils/continuous_storage.h"
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
    virtual ~TClass(){}
};

struct SClassA: public TClass {
    SClassA(): TClass(1){}
};
struct SClassB: public TClass {
    SClassB(): TClass(2){}
};
struct SClassC: public TClass {
    uint32_t c_attr=0;
    SClassC(): TClass(3) {};
    SClassC(uint32_t value):
     TClass(3), c_attr(value){}

     ~SClassC() override {LOG_INFO("SClassC Destructor " << c_attr << ".");}
};

void test_subclasses_map1() {
    auto sclassmap = SubClassMap<TClass>();

    sclassmap.register_type<SClassA>();
    sclassmap.register_type<SClassB>();
    sclassmap.register_type<SClassC>();

    auto a = SClassA();
    auto b = SClassB();
    auto c = SClassC(9);

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
void test_subclasses_map2() {
    LOG_INFO("test subclasses map2");
    auto sclassmap = SubClassMap<TClass>();

    sclassmap.register_type<SClassA>();
    sclassmap.register_type<SClassB>();
    sclassmap.register_type<SClassC>();

    auto a = SClassA();

    sclassmap.push_back<SClassA>(&a);
    sclassmap.push_back<SClassB>(SClassB());
    sclassmap.emplace_back<SClassC>(9);

    LOG_INFO("-All classes values-");
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


void test_continuous_storage() {
    LOG_INFO("test Continuous storage");
    auto strg = BufferStorage<TClass>();

    auto a = SClassC(1);
    auto b = SClassC(2);
    auto c = SClassC(3);
    auto d = SClassC(4);

    auto aval = strg.insert(a);
    auto bval = strg.insert(std::move(b));
    auto cval = strg.insert(c);
    auto dval =  strg.insert(d);

    LOG_INFO(aval->c_attr);
    assert(aval->c_attr == 1 && "Attr should be 1!");

    LOG_INFO(bval->c_attr);
    assert(bval->c_attr == 2 && "Attr should be 2!");

    LOG_INFO(cval->c_attr);
    assert(cval->c_attr == 3 && "Attr should be 3!");

    LOG_INFO(dval->c_attr);
    assert(dval->c_attr == 4 && "Attr should be 4!");

//    auto dt = (SClassC*) strg.data();
//    LOG_INFO(dt->c_attr);
}


int main() {
//    _test_text_template();
//    _test_shader_parser();
//    _test_sizes();
//    test_value();
//    test_multiset();

//    test_vectorit();
//    test_mapIteration();
//    test_subclasses_map2();

    test_continuous_storage();

    return 0;

}