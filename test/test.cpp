//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "Pong/Core/shader_parser.h"
#include <fstream>
#include <vector>
#include <string>


int main() {
    auto shaders = shaders_from_file("./test/parser_shader.glsl");
    LOG_INFO(shaders.find(ShaderType::VERTEX)->second.get_data());

    auto shader_map = ShaderMap();

    LOG_INFO("Max bucket count: " << shader_map.max_bucket_count());
    LOG_INFO("Max size: " << shader_map.max_size());

    LOG_INFO("Bucket count: " << shaders.bucket_count());
//    LOG_INFO("Bucket: " << shaders.)

    shader_map[ShaderType::VERTEX] = ShaderParser();
    shader_map[ShaderType::VERTEX] = shaders[ShaderType::VERTEX];
    shader_map[ShaderType::VERTEX] = shaders[ShaderType::FRAGMENT];

    for (auto k: {ShaderType::VERTEX, ShaderType::FRAGMENT, ShaderType::GEOMETRY, ShaderType::NONE}) {
        LOG_INFO("Hash value: " << shaders.hash_function()(k));
    }

    LOG_INFO(std::string(20, '-'));

    LOG_INFO(shader_map[ShaderType::VERTEX].get_data().c_str());


}