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
#include <cstdlib>
#include "utils/action_event.h"
#include "utils/text_template.h"
#include "utils/shader_parser.h"
#include "utils/subclasses_map.h"
#include "utils/fixed_address_buffer.h"
#include "utils/logger.h"
#include "Pong/core/command.h"
//#include "Pong/core/primitive_component.h"
//#include "Pong/core/component.h"

//template<typename Derived_>
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

//void _test_sizes() {
//    LOG_INFO("position size " << sizeof(Pong::Vertex::position));
//    LOG_INFO("OFFSET texcoords " << offsetoff(Pong::Vertex, tex_coords));
//}

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
    auto strg = FixedAddressBuffer<TClass>();

    auto a = SClassC(1);
    auto b = SClassC(2);
    auto c = SClassC(3);
    auto d = SClassC(4);

    auto aval = strg.insert(a);
    auto bval = strg.insert(std::move(b));
    auto cval = strg.insert(c);
    auto dval =  strg.insert(d);
    auto e_val =  strg.emplace<SClassC>(5);

//    auto p_val =  strg.insert(6);

    LOG_INFO(aval->c_attr);
    assert(aval->c_attr == 1 && "Attr should be 1!");

    LOG_INFO(bval->c_attr);
    assert(bval->c_attr == 2 && "Attr should be 2!");

    LOG_INFO(cval->c_attr);
    assert(cval->c_attr == 3 && "Attr should be 3!");

    LOG_INFO(dval->c_attr);
    assert(dval->c_attr == 4 && "Attr should be 4!");

    LOG_INFO(e_val->c_attr);
    assert(e_val->c_attr == 5 && "Attr should be 5!");
}

namespace {
    void _test_func_a(int a) {
        LOG_INFO("test func a: " << a);
    }

    void _test_func_b(int a) {
        LOG_INFO("test func b: " << a);
    }

    class _tstclass {
    public:
        void print_val(int a) {
            LOG_INFO("_tstclass.print_val: " << a);
        }
    };

    template<typename T>
    void forwarder(void* context, int a) {
        static_cast<T>(context)->print_val(a);
    }
}

void test_action_event() {
    LOG_INFO("test Continuous storage");

//    auto act = Event<decltype(_test_func)>{};
    auto act = Event<void(*)(int)>{};
    act += &_test_func_a;

    act.subscribe(&_test_func_b);
    act(2);

    auto inst = _tstclass();
    auto act2 = Event<void(*)(void*, int)>{};
    act2.subscribe(&(forwarder<_tstclass*>));
    act2(&inst, 4);

}

constexpr uint32_t la_array() {
//    std::srand(std::time(nullptr));
    return 10;
}


namespace Pong {
    class Actor;
}

//class DComponent: public Pong::Component {
//public:
//    inline void start(Pong::Actor* actor, Pong::Component* parent) override {}
//    inline void update(Pong::Actor* actor, Pong::Component* parent) override {}
//};

void test_component() {

//    auto compo = DComponent();

//    compo.subscribe(new DComponent);

}

class CBase {
public:
    using c_str = const char*;
public:
    inline static c_str name="CBase";
};

class CDerived: public CBase {
public:
    uint32_t number = 0;
    inline static c_str name="CDerived";

};

void test_static_modifications() {
    CBase* drived = new CDerived;
    drived->name = "a";

    LOG_INFO("Derived instance name " << drived->name);
    LOG_INFO("Base name " << CBase::name);
    LOG_INFO("Derived name " << CDerived::name);
}

class TCommand: public Pong::Command {
    COMMAND_SETUP("derived_command", TCommand);

public:
    void doit() override {
        LOG_INFO("Do it command");
    }

};

void test_command_register() {
    auto c_reg = Pong::CommandReg();
    LOG_INFO(TCommand::name);

    c_reg.register_command<TCommand>();
    c_reg.create_command(TCommand::name)->doit();

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

//    test_continuous_storage();
//    test_action_event();

    test_command_register();


    return 0;
}