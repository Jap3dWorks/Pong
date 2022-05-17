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
#include <cassert>

class ShaderParser {
private:
    std::string _data{};

    std::regex _include_pattern {R"(#include\s+[<"]([^<>"]+)[>"])"};
    std::regex _version_pattern {R"(#version\s+(\d+))"};
    std::regex _shader_pattern {R"(#shader\s+(\w+))"};

    bool _empty{true};

public:
    _P_INLINE _P_STATIC const char* glsl_version{"4.5"};

private:
    void _get_file_data(std::stringstream& out_result, std::ifstream& input_stream) {

        std::string line;
        std::smatch base_match;
        uint32_t count = 0;
        while (std::getline(input_stream, line)) {
            if (std::regex_search(line, base_match, _include_pattern)) {
                _get_file_data(out_result, base_match[1].str().c_str());
            }
            else if(std::regex_search(line, base_match, _version_pattern)) {
                LOG_DEBUG("-Exclude #version line-");
            }
            else if (std::regex_search(line, base_match, _shader_pattern)) {
                input_stream.seekg(count);
                break;
            }
            else {
                out_result << line << "\n";
                _empty = false;
            }
            count++;
        }
    }

    void _get_file_data(std::stringstream& out_result, const char* file_path) {
        auto input_stream = std::ifstream(file_path);
        assert(input_stream.good());

        _get_file_data(out_result, input_stream);
    }

    _P_INLINE std::string _get_file_data(const char* file_path) {
        auto result = _initialize_string_stream();
        _get_file_data(result, file_path);
        return result.str();
    }

    _P_NODISCARD _P_INLINE std::stringstream _initialize_string_stream() {
        std::stringstream ss_data;
        ss_data << "#version " << glsl_version << "<\n";
        return ss_data;
    }

public:
    _P_EXPLICIT ShaderParser(std::string&& data) : _data(std::move(data)), _empty(data.empty()) {}
    _P_EXPLICIT ShaderParser(const std::string& data) : _data(data), _empty(data.empty()) {}

    ShaderParser(const ShaderParser& other): _data(other._data), _empty(other._empty) {}

    ShaderParser(ShaderParser && other) noexcept : _data(std::move(other._data)), _empty(other._empty) {
    }

    ~ShaderParser()=default;

    ShaderParser& operator=(const ShaderParser& other) {
        _data = other._data;
        _empty = other._empty;

        return *this;
    }

    ShaderParser& operator=(ShaderParser&& other)  noexcept {
        _data = std::move(other._data);
        _empty = other._empty;

        other._empty = true;

        return *this;
    }

    _P_EXPLICIT ShaderParser(const char* file_path) {
        _data = _get_file_data(file_path);
    };

    _P_EXPLICIT ShaderParser(std::ifstream& file_stream) {
        auto t_data = _initialize_string_stream();
        _get_file_data(t_data, file_stream);
        _data = t_data.str();
    }

public:
    _P_NODISCARD _P_CONST std::string& get_data() _P_CONST {
        return _data;
    }

    _P_NODISCARD bool empty() _P_CONST {
        return _empty;
    }

    void clear() {
        _data = "";
        _empty = true;
    }

};




#endif //GL_TEST_SHADER_PARSER_H
