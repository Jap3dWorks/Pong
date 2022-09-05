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


    template<typename T>
    typename from_hash<T>::type get_typename(T& type) {
        return from_hash<T>::type();
    }

    void test_get_component() {

        auto datacontroller = pong::RegDataController();

        using Range = boost::mpl::range_c<uint32_t, 1, COUNTER_READ(component_tag) + 1>;

        boost::mpl::for_each<Range>(
            [&]<typename I>(I i) constexpr -> void {
                LOG_INFO("" << typeid(typename from_hash<hash_number<I::value>>::type).name());

                datacontroller.reg_type<typename from_hash<hash_number<I::value>>::type>();
            }
        );

        datacontroller.insert_type<TComp2>({2}, {});

        auto compo =  datacontroller.get_type<TComp2>({2});

        LOG_INFO("compo val: " << compo.attr);
    }

}

#endif //PONG_TEST_METAP_TESTS_H_
