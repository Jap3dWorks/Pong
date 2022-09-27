//
// Created by Jordi on 9/10/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZER_H_
#define PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZER_H_

#include "Utils/logger.h"
#include "Pong/components/component.h"
#include "Pong/registers/reg_data_controller.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/bind.hpp>


namespace pong::serializer {

    template<int N>
    struct ComponentList {
        size_t arr[N];

        constexpr ComponentList(): arr() {
            using Range = boost::mpl::range_c<uint32_t, 1, N + 1>;
            boost::mpl::for_each<Range>(
                [&]<typename I>(I i) constexpr -> void {
                    arr[I::value - 1] = component::components_collected<I::value>::hash::value;
                }
            );
        }
    };


    template<typename T>
    void deserialize_component(std::ifstream & file_path,
                               size_t hash_number,
                               RegDataController & controller) {
        if (component::to_hash<T>::hash::value == hash_number) {

            LOG_INFO(typeid(T).name());
            LOG_INFO("Hash value:" << component::to_hash<T>::hash::value);
        }
    }

    template<typename T>
    struct DeserializeComponents;

    template<template<typename ...Args> typename T, typename ...Args>
    struct DeserializeComponents<T<Args...>> {
        std::ifstream &stream;
        RegDataController &reg_controller;

        DeserializeComponents(std::ifstream &file_stream, RegDataController &controller):
            stream(file_stream), reg_controller(controller) {
            // get hash here
            while (file_stream.is_open() || true) {
                (deserialize_component<Args>(file_stream, 10), ...);
                file_stream.close();
                break;
            }
        }
    };


    void serialize_components(RegDataController& reg_controller) {
    }

}
#endif //PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZER_H_
