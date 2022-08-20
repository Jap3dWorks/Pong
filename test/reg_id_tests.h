//
// Created by Jordi on 8/20/2022.
//

#ifndef PONG_REG_ID_TESTS_H
#define PONG_REG_ID_TESTS_H

#include "Pong/registers/reg_data_controller.h"
#include "Pong/file_data/serial_data.h"
#include "Pong/core/geometry_data.h"
#include "Utils/type_conditions.h"
#include "Utils/logger.h"

#include <iostream>


struct TestType{
    const char* Name;
    uint32_t data;
};

void test_variadic_class_access () {
    auto n = variadic_id_t<3, uint32_t, uint8_t, uint16_t, uint64_t>::index;
    LOG_INFO("Index " << n);
    variadic_id_t<3, uint32_t, uint8_t, uint16_t, TestType>::type_t a{"pepe", 10};
    LOG_INFO("Type " << typeid(decltype(a)).name());

    LOG_INFO("Data " << a.Name << " " << a.data);

}


void test_reg_id_data() {
    auto reg_data = Pong::RegDataController<Pong::RegDataLocation<Pong::Mesh>>();

    reg_data.



}


#endif //PONG_REG_ID_TESTS_H
