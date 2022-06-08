//
// Created by Jordi on 5/17/2022.
//

#ifndef GL_TEST_SHADER_PARSER_H
#define GL_TEST_SHADER_PARSER_H

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <utility>
#include <sstream>
#include "Pong/Core/core_vals.h"
#include "Pong/logger.h"
#include "text_template.h"
#include <cassert>
#include <map>
#include <unordered_map>

using BasicShaderType = uint8_t;
enum class ShaderType: BasicShaderType {
    NONE=0,
    VERTEX=1,
    TESS_CONTROL=2,
    TESS_EVALUATION=3,
    GEOMETRY=4,
    FRAGMENT=5,
    COMPUTE=6

};

template<>
struct std::hash <ShaderType> {
    std::size_t operator()(const ShaderType& _value) const noexcept {
        return std::hash<BasicShaderType>()(static_cast<BasicShaderType>(_value));
    }
};

namespace {
    using NameShaderMap = _P_CONST std::unordered_map<std::string, ShaderType>;
    using ShaderTemplateMap = _P_CONST std::unordered_map<ShaderType, std::string>;
    using TemplateVariableMap = _P_CONST std::unordered_map<std::string, std::string>;

    // Do not use global variables
    _P_INLINE NameShaderMap& get_name_shader_map() {
        _P_STATIC NameShaderMap _shader_types = {
            {"vertex",   ShaderType::VERTEX},
            {"tess_control", ShaderType::TESS_CONTROL},
            {"tess_evaluation", ShaderType::TESS_EVALUATION},
            {"fragment", ShaderType::FRAGMENT},
            {"geometry", ShaderType::GEOMETRY},
            {"compute", ShaderType::COMPUTE}
        };
        return _shader_types;
    }

    _P_INLINE ShaderTemplateMap& get_shader_template_map() {
        _P_STATIC ShaderTemplateMap _shader_template {
                {ShaderType::NONE, "default_shader.template"},
                {ShaderType::VERTEX, "vertex_shader.template"},
                {ShaderType::TESS_CONTROL, "tess_control.template"},
                {ShaderType::TESS_EVALUATION, "tess_evaluation.template"},
                {ShaderType::FRAGMENT, "fragment_shader.template"},
                {ShaderType::GEOMETRY, "geometry_shader.template"},
                {ShaderType::COMPUTE, "compute_shader.template"}
        };
        return _shader_template;
    }

//    _P_INLINE TemplateVariableMap& get_template_variable_map() {
//        _P_STATIC TemplateVariableMap _template_variable_map {
//                {"vertex_shader.template", }
//        }
//    }

}

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
};

class ShaderParser {
private:
    std::string _data{};
    ShaderType _shader_type{ShaderType::NONE};

    std::regex _include_pattern {R"(#include\s+[<"]([^<>"]+)[>"])"};
    std::regex _version_pattern {R"(#version\s+(\d+))"};
    std::regex _shader_pattern {R"(#shader\s+(\w+))"};

public:
    _P_INLINE _P_STATIC _P_CONST char* shaders_path{"./Shaders/"};
    _P_INLINE _P_STATIC _P_CONST char* templates_dir{"./Shaders/templates"};

private:
    auto _P_INLINE _get_shader_code(std::ifstream &input_stream) -> std::stringstream {
        std::string line;
        std::smatch base_match;
        auto result = std::stringstream();
        auto last_pos = input_stream.tellg();

        while (std::getline(input_stream, line)) {
            if (std::regex_search(line, base_match, _shader_pattern)) {
                if (_shader_type == ShaderType::NONE) {
                    _shader_type = get_name_shader_map().at(base_match[1].str());
                } else {
                    input_stream.seekg(last_pos);
                    break;
                }

            }
            else {
                result << line << "\n";
            }

            last_pos = input_stream.tellg();
        }

        return result;
    }

    auto _P_INLINE _resolve_shader_code(std::string shader_code) -> std::string {
        std::string line;
        std::smatch base_match;

        auto start = shader_code.cbegin();
        uint32_t offset = 0;

        while(std::regex_search(start, shader_code.cend(), base_match, _include_pattern)) {
            auto _path = base_match[1].str();
            offset += base_match.position();

            auto _replace = _get_file_string(shaders_path + _path);

            // TODO here fix replace
            shader_code.replace(
                    offset,
                    offset + base_match[0].length(),
                    _replace
            );
            start += offset;
//            end = shader_code.cend();
        }

        return shader_code;
    }

    auto _P_INLINE _get_file_string(const std::string& file_path) -> std::string {
        auto shader_file = std::ifstream(file_path);
        auto string_stream = std::stringstream();
        string_stream << shader_file.rdbuf();

        return string_stream.str();
    }

    static std::string _P_INLINE _render_template(
            ShaderType shader_type,
            const std::string& shader_code) {
        auto _template = get_shader_template_map().at(shader_type);
        std::stringstream string_stream;
        try {
            string_stream << std::ifstream(
                    std::string(templates_dir) + "/" + _template
                    ).rdbuf();

        } catch(const std::exception& e) {
            LOG_ERROR(e.what());
            return "";
        }

        return TextTemplate(string_stream.str(),
            {
                {"code", {shader_code, {}}},

            }
        ).render();
    }

public:
    ShaderParser()=default;

    ShaderParser(const ShaderParser& other):
    _data(other._data), _shader_type(other._shader_type) {}

    ShaderParser(ShaderParser && other) noexcept :
    _data(std::move(other._data)), _shader_type(other._shader_type) {}

    _P_EXPLICIT ShaderParser(std::ifstream &file_stream) {
        assert(set_data(file_stream));
    }

    ~ShaderParser()=default;

    ShaderParser &operator=(const ShaderParser &other) {
        _data = other._data;
        return *this;
    }

    ShaderParser &operator=(ShaderParser &&other) noexcept {
        _data = std::move(other._data);
        return *this;
    }

public:
    bool _P_INLINE set_data(std::ifstream &file_stream) {
        _shader_type = ShaderType::NONE;

        auto shader_code = _get_shader_code(file_stream);

        if (_shader_type == ShaderType::NONE) {
            LOG_ERROR("SHADER type not found!");
            return false;
        }

        _data = _resolve_shader_code(
                _render_template(_shader_type,
                                 shader_code.str())
        );

        return true;
    }

    _P_NODISCARD _P_CONST std::string& get_data() _P_CONST noexcept {
        return _data;
    }

    _P_NODISCARD bool empty() _P_CONST {
        return _data.empty();
    }

    _P_NODISCARD auto get_type() _P_CONST {
        return _shader_type;
    }

    void clear() {
        _data = "";
    }

};


using ShaderMap = std::unordered_map<ShaderType, ShaderParser>;

_P_INLINE ShaderMap shaders_from_file(const char* file_path) {
    auto result = ShaderMap();
    auto shader_stream = std::ifstream(file_path);
    while(shader_stream) {
        auto shader_parser = ShaderParser(shader_stream);
        result[shader_parser.get_type()] = std::move(shader_parser);
    }
    return result;
}



#endif //GL_TEST_SHADER_PARSER_H
