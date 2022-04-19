//#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "../logger.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
};

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
};

ShaderProgramSource ParseShader(const std::string& FilePath)
{
    std::ifstream stream(FilePath);
    std::stringstream ss[2];

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    ShaderType type = ShaderType::NONE;
    std::string line;
    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str(), ss[2].str()};
}

unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    return 0;

}

class Shader
{
    // the program id
public:
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {
        // retrieve the vertex/fragment source code from filepath
        std::stringstream ss[2];

        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        std::ifstream stream(vertexPath);
        ShaderType type = ShaderType::NONE;
        std::string line;
        while(getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                ss[(int)type] << line << "\n";
            }
        }
        vertexCode = ss[0].str();
        fragmentCode = ss[1].str();
        geometryCode = ss[2].str();

//        std::ifstream vShaderFile;
//        std::ifstream fShaderFile;
//        std::ifstream gShaderFile;
//        // ensure ifstream objects can throw excetions:
//        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//        try
//        {
//            // TODO: one file shader
//            // open files
//            vShaderFile.open(vertexPath);
//            fShaderFile.open(fragmentPath);
//            std::stringstream vShaderStream, fShaderStream;
//
//            // read files content into streams
//            vShaderStream << vShaderFile.rdbuf();
//            fShaderStream << fShaderFile.rdbuf();
//
//            //clase file handlers
//            vShaderFile.close();
//            fShaderFile.close();
//
//            // convert stream into string
//            vertexCode = vShaderStream.str();
//            fragmentCode = fShaderStream.str();
//
//            // if geometry shader is present, also load
//            if (geometryPath != nullptr)
//            {
//                gShaderFile.open(geometryPath);
//                std::stringstream gShaderStream;
//                gShaderStream << gShaderFile.rdbuf();
//                gShaderFile.close();
//                geometryCode = gShaderStream.str();
//            }
//
//        }
//        catch (std::ifstream::failure e)
//        {
//            LOG_ERROR(vertexPath)
//            LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCEFULLY_READ")
//        }

        ShaderProgramSource source = ParseShader(vertexPath);

        // to c type
        const char * vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "VERTEX");
        // if geometry shader is given compile
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, nullptr);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
        {
            glAttachShader(ID, geometry);
        }
        glLinkProgram(ID);
        // print link errors if any
        checkCompileErrors(ID, "PROGRAM");
        // delete shaders once they're linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
        {
            glDeleteShader(geometry);
        }
    }

    // use/activate the shader
    void use() const {
        glUseProgram(ID);
    }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string &name,const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


private:
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog;
            }
        }
    }
};

#endif // !SHADER_H

