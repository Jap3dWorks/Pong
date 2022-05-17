//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "Pong/Core/shader_parser.h"

int main() {

    auto shdr = ShaderParser("./test/parser_shader.glsl");
    std::cout << shdr.get_data() << std::endl;

}