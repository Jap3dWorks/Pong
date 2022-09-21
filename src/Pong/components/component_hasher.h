//
// Created by Jordi on 9/4/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_COMPONENT_HASHER_H_
#define PONG_SRC_PONG_SERIALIZER_COMPONENT_HASHER_H_

#include <iostream>
#include <utility>
#include "Utils/type_conditions.h"
#include "Utils/hash_utils.h"

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/bind.hpp>


namespace pong::component {


    // -- Component serialization test
    template<size_t N>
    struct constant_index : std::integral_constant<std::size_t, N> {};

    template<typename id, std::size_t rank, std::size_t acc>
    constexpr constant_index<acc> counter_crumb(id, constant_index<rank>, constant_index<acc>) {
        return {};
    }


#define COUNTER_READ_CRUMB(TAG, RANK, ACC) pong::component::counter_crumb(TAG(), pong::component::constant_index<RANK>(), pong::component::constant_index<ACC>())

#define COUNTER_READ(TAG) COUNTER_READ_CRUMB(TAG, 1, \
                                COUNTER_READ_CRUMB(TAG, 2, \
                                  COUNTER_READ_CRUMB(TAG, 4, \
                                    COUNTER_READ_CRUMB(TAG, 8, \
                                      COUNTER_READ_CRUMB(TAG, 16, \
                                        COUNTER_READ_CRUMB(TAG, 32, \
                                          COUNTER_READ_CRUMB(TAG, 64, \
                                            COUNTER_READ_CRUMB(TAG, 128, 0))))))))

#define COUNTER_INC(TAG) \
       constant_index<COUNTER_READ(TAG) + 1> \
       constexpr counter_crumb(TAG, constant_index<(COUNTER_READ(TAG) + 1) & ~COUNTER_READ(TAG) > , \
            constant_index<(COUNTER_READ(TAG) + 1) & COUNTER_READ(TAG)>) {return {};}


    struct component_tag {};


    // --
    template<size_t N>
    struct hash_number: std::integral_constant<size_t, N> {};

    template<typename T>
    struct to_hash {
        using hash = hash_number<0>;
    };

    template<typename T>
    struct from_hash {};


    template<size_t N, typename ...Args>
    struct all_components {
        static constexpr size_t count{0};
    };


    template<typename ...Args>
    struct ArgsType {};

    template<typename T, typename ...Args>
    struct CombineTypes {};

    template<template<typename ...Args> typename T, typename ...Args, typename ...Extra>
    struct CombineTypes<T<Args...>, Extra...> {
        using types = ArgsType<Args..., Extra...>;

        template<uint32_t N>
        using variadic_types = variadic_id_t<N, Extra..., Args...>;

//        using component_type = variadic_id_t<0, Extra...>;

        static constexpr size_t count = sizeof...(Args) + sizeof...(Extra);
    };

    template<uint32_t N>
    struct components_collected {
        using combine_types = CombineTypes<ArgsType<std::false_type>>;
    };


#define REG_COMPONENT(component) \
    COUNTER_INC(component_tag) \
    template<> \
    struct to_hash<component> {  \
        using hash = hash_number<COMPILE_TIME_CRC32_STR(#component)>; \
    }; \
    template<> \
    struct from_hash<hash_number<COMPILE_TIME_CRC32_STR(#component)>> { \
        using type = component; \
    }; \
    template<> \
    struct components_collected<COUNTER_READ(component_tag)> { \
        using combine_types = CombineTypes<components_collected<COUNTER_READ(component_tag) - 1>::combine_types::types, component>; \
        using hash = hash_number<COMPILE_TIME_CRC32_STR(#component)>; \
        using type = component; \
    };

}

#endif //PONG_SRC_PONG_SERIALIZER_COMPONENT_HASHER_H_
