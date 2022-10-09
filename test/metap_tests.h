//
// Created by Jordi on 8/30/2022.
//

#ifndef PONG_TEST_METAP_TESTS_H_
#define PONG_TEST_METAP_TESTS_H_

#include "Pong/serializer/_dep/component_serializer.h"

#include <utility>
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


    void test_get_component() {
        auto datacontroller = pong::RegDataController();
        using Range = boost::mpl::range_c<uint32_t, 1, COUNTER_READ(pong::component::component_tag) + 1>;

        boost::mpl::for_each<Range>(
            [&]<typename I>(I i) constexpr -> void {
                datacontroller.reg_type<typename pong::component::components_collected<I::value>::type>();
            }
        );

        datacontroller.insert_type<pong::component::TransformComp>({2}, {});
        auto compo = datacontroller.get_type<pong::component::TransformComp>({2});

        auto complist = pong::serializer::ComponentList<COUNTER_READ(pong::component::component_tag)>();
        for (uint32_t i = 0; i < COUNTER_READ(pong::component::component_tag); ++i) {
            LOG_INFO(complist.arr[i]);
        }

        auto controller = pong::RegDataController();
        auto file_stream = std::ifstream();
        auto deserializer = pong::serializer::DeserializeComponents<
            pong::component::components_collected<COUNTER_READ(pong::component::component_tag)>::combine_types::types
            >(file_stream, controller);

        LOG_INFO("End function");
//        from_hash<hash_number<complist.arr[0]>>::type;
    }

}

#endif //PONG_TEST_METAP_TESTS_H_
