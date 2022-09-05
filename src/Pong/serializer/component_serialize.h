//
// Created by Jordi on 9/4/2022.
//

#ifndef PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZE_H_
#define PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZE_H_
#include <iostream>
#include <utility>
#include "Utils/type_conditions.h"

namespace pong::serializer {


    // -- Component serialization test
    template<size_t N>
    struct constant_index : std::integral_constant<std::size_t, N> {};

    template<typename id, std::size_t rank, std::size_t acc>
    constexpr constant_index<acc> counter_crumb(id, constant_index<rank>, constant_index<acc>) {
        return {};
    }


#define COUNTER_READ_CRUMB(TAG, RANK, ACC) counter_crumb(TAG(), constant_index<RANK>(), constant_index<ACC>())

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
        static inline constexpr size_t hash=0;
    };

    template<typename T>
    struct from_hash {};


    template<size_t N, typename ...Args>
    struct all_components {
        static constexpr size_t count{0};
    };


    template<typename ...Args>
    struct typesA {};

    template<typename T, typename ...Args>
    struct combine_types {};

    template<template<typename ...Args> typename T, typename ...Args, typename ...Extra>
    struct combine_types<T<Args...>, Extra...> {
        using types = typesA<Args..., Extra...>;

        template<uint32_t N>
        using variadic_types = variadic_id_t<N, Extra..., Args...>;

//        using component_type = variadic_id_t<0, Extra...>;

        static constexpr size_t count = sizeof...(Args) + sizeof...(Extra);
    };

    template<uint32_t N>
    struct components_collected {
        using types = combine_types<typesA<uint32_t>, uint32_t>;
    };

    combine_types<typesA<>, uint32_t>::variadic_types<0> get_component_hash(hash_number<0>) {return {};}


#define REG_COMPONENT(component) \
    COUNTER_INC(component_tag) \
    template<> \
    struct to_hash<component> { \
        static inline constexpr hash_number<COUNTER_READ(component_tag)> hash; \
    }; \
    template<> \
    struct from_hash<hash_number<COUNTER_READ(component_tag)>> { \
        using type = component; \
    };                           \
    template<>                                      \
    struct components_collected<COUNTER_READ(component_tag)> {                 \
        using types = combine_types<components_collected<COUNTER_READ(component_tag) - 1>::types::types, component>; \
    };



#define COMPONENT_TYPE(hash) \
    from_hash<hash>::type

#define COMPONENT_HASH(type) \
    to_hash<type>::hash





}

#endif //PONG_SRC_PONG_SERIALIZER_COMPONENT_SERIALIZE_H_
