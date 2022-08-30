//
// Created by Jordi on 8/30/2022.
//

#ifndef PONG_TEST_METAP_TESTS_H_
#define PONG_TEST_METAP_TESTS_H_

#include "Utils/type_conditions.h"
#include "Utils/logger.h"

//namespace test::metap {
    template<typename T>
    struct TTemplate {};

    using t_type = TTemplate<bool>;

    template<templated_from<TTemplate> T>
    struct only_from_ttemplate{};


    using test_templated = only_from_ttemplate<t_type>;


    void test_tesmplated_from_concept() {
    }

//}

#endif //PONG_TEST_METAP_TESTS_H_
