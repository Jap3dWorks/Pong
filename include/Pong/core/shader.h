//#pragma once
#ifndef SHADER_H
#define SHADER_H

// #include <yvals_core.h>
#include "Pong/core/core_vals.h"
#include "Utils/logger.h"
#include "Utils/shader_parser.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

_P_INLINE unsigned int CompileShader(unsigned int type, _P_CONST std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    return 0;

}

class Shader {
    // the program id
public:
    unsigned int ID;

    // constructor reads and builds the shader
    _P_EXPLICIT Shader(const char *shader_path) {
        // retrieve the vertex/fragment source code from filepath
        auto parse_shader = shaders_from_file(shader_path);

        // to c Type
        const char *vShaderCode = parse_shader[ShaderType::VERTEX].get_data().c_str();
        const char *fShaderCode = parse_shader[ShaderType::FRAGMENT].get_data().c_str();

        // compile shaders
        uint32_t vertex, fragment,
                tess_control, tess_evaluation,
                geometry, compute;

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        _check_compile_errors(vertex, "POSITION");

        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        _check_compile_errors(fragment, "FRAGMENT");

        // shader program
        ID = glCreateProgram();

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        if (parse_shader.contains(ShaderType::TESS_CONTROL)) {
            const char *shader_code = parse_shader[
                    ShaderType::TESS_CONTROL].get_data().c_str();
            tess_control = glCreateShader(GL_TESS_CONTROL_SHADER);
            glShaderSource(tess_control, 1, &shader_code, nullptr);
            glCompileShader(tess_control);
            _check_compile_errors(tess_control, "TESS_CONTROL");
            glAttachShader(ID, tess_control);
            glDeleteShader(tess_control);
        }

        if (parse_shader.contains(ShaderType::TESS_EVALUATION)) {
            const char *shader_code = parse_shader[
                    ShaderType::TESS_EVALUATION].get_data().c_str();
            tess_evaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
            glShaderSource(tess_evaluation, 1, &shader_code, nullptr);
            glCompileShader(tess_evaluation);
            _check_compile_errors(tess_evaluation, "TESS_EVALUATION");
            glAttachShader(ID, tess_evaluation);
            glDeleteShader(tess_evaluation);
        }

        if (parse_shader.contains(ShaderType::GEOMETRY)) {
            const char *shader_code = parse_shader[
                    ShaderType::GEOMETRY].get_data().c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &shader_code, nullptr);
            glCompileShader(geometry);
            _check_compile_errors(geometry, "GEOMETRY");
            glAttachShader(ID, geometry);
            glDeleteShader(geometry);
        }

        if (parse_shader.contains(ShaderType::COMPUTE)) {
            const char *shader_code = parse_shader[
                    ShaderType::COMPUTE].get_data().c_str();
            compute = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(compute, 1, &shader_code, nullptr);
            glCompileShader(compute);
            _check_compile_errors(compute, "COMPUTE");
            glAttachShader(ID, compute);
            glDeleteShader(compute);
        }

        glLinkProgram(ID);
        // print link errors if any
        _check_compile_errors(ID, "PROGRAM");
        // delete shaders once they're linked
    }

    // use/activate the shader
    void use() const {
        glUseProgram(ID);
    }

    // utility uniform functions
    void set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void set_int(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_float(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void set_vec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void set_vec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void set_vec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void set_vec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void set_vec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void set_vec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void set_mat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void set_mat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


private:
    _P_STATIC void _check_compile_errors(GLuint shader, const std::string &type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of Type: " << type << "\n" << infoLog;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of Type: " << type << "\n" << infoLog;
            }
        }
    }
};

#endif // !SHADER_H

