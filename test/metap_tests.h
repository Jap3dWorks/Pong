//
// Created by Jordi on 8/30/2022.
//

#ifndef PONG_TEST_METAP_TESTS_H_
#define PONG_TEST_METAP_TESTS_H_

#include "Pong/serializer/component_serialize.h"
#include "Utils/type_conditions.h"
#include "Utils/logger.h"
#include <utility>
#include "Pong/registers/reg_data_controller.h"
#include <unordered_map>

#include <fstream>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/bind.hpp>


namespace metap_tests {

    template<typename T>
    struct TTemplate {};

    using t_type = TTemplate<bool>;

    template<templated_from<TTemplate> T>
    struct only_from_ttemplate {};

    using test_templated = only_from_ttemplate<t_type>;
}

    namespace pong::serializer {

    struct TComp1{
        uint32_t attr{1};
    };
    REG_COMPONENT(TComp1);

    struct TComp2 {
        uint32_t attr{2};
    };
    REG_COMPONENT(TComp2);


    template<int N>
    struct ComponentList {
        size_t arr[N];

        constexpr ComponentList(): arr() {
            using Range = boost::mpl::range_c<uint32_t, 1, N + 1>;
            boost::mpl::for_each<Range>(
                [&]<typename I>(I i) constexpr -> void {
                    arr[I::value - 1] = components_collected<I::value>::hash::value;
                }
            );
        }
    };


    template<typename T>
    void deserialize_component(std::ifstream & file_path, size_t hash_number) {
        if (to_hash<T>::hash::value) {
            LOG_INFO(typeid(T).name());
            LOG_INFO("Hash value:" << to_hash<T>::hash::value);
        }
    }

    template<typename T>
    struct DeserializeComponents;

    template<template<typename ...Args> typename T, typename ...Args>
    struct DeserializeComponents<T<Args...>>{
        std::ifstream &stream;
        RegDataController &reg_controller;

        DeserializeComponents(std::ifstream &file_path, RegDataController &controller):
            stream(file_path), reg_controller(controller) {
            // get hash here
            while (file_path.is_open() || true) {
                (deserialize_component<Args>(file_path, 10), ...);
                file_path.close();
                break;
            }
        }
    };

    void test_get_component() {
        auto datacontroller = pong::RegDataController();
        using Range = boost::mpl::range_c<uint32_t, 1, COUNTER_READ(component_tag) + 1>;

        boost::mpl::for_each<Range>(
            [&]<typename I>(I i) constexpr -> void {
                datacontroller.reg_type<typename components_collected<I::value>::type>();
            }
        );

        datacontroller.insert_type<TComp2>({2}, {});
        auto compo = datacontroller.get_type<TComp2>({2});
        LOG_INFO("compo val: " << compo.attr);

        auto complist = ComponentList<COUNTER_READ(component_tag)>();
        for (uint32_t i = 0; i < COUNTER_READ(component_tag); ++i) {
            LOG_INFO(complist.arr[i]);
        }

        auto controller = RegDataController();
        auto file_stream = std::ifstream();
        auto deserializer = DeserializeComponents<components_collected<COUNTER_READ(component_tag)>::combine_types::types>(
            file_stream, controller);

        LOG_INFO("End function");
//        from_hash<hash_number<complist.arr[0]>>::type;
    }

}

#endif //PONG_TEST_METAP_TESTS_H_
