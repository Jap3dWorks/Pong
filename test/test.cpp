//
// Created by Jordi on 4/23/2022.
//

#include <iostream>
#include <string>
#include <regex>
#include <type_traits>

#include "Utils/logger.h"

//#include "reg_id_tests.h"
//#include "base_tests.h"
//#include "basic_tests.h"
#include "filedata_tests.h"
//#include "metap_tests.h"
#include "type_tests.h"





int main() {
//    base_tests::base_tests();
//    pong::serializer::test_get_component();
    filedata_tests();
    type_tests();

    LOG_INFO("--Finalize tests--");

    return 0;
}