//
// Created by Jordi on 3/7/2021.
//
// https://www.youtube.com/watch?v=KyCQBQzaBOM&list=PLlrATfBNZ98f5vZ8nJ6UengEkZUMC4fy5&index=8&ab_channel=TheCherno
#ifndef PONG_SANDBOX_H
#define PONG_SANDBOX_H

#include "Core/Level.h"
#include "Core/core_vals.h"
#include <glm/glm.hpp>
#include <array>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// create batch rendering

_P_CONSTEXPR uint32_t max_quads = 1000;

class Color{
public:
    _P_STATIC _P_INLINE _P_CONSTEXPR glm::vec4 white = {1.f, 1.f, 1.f, 1.f};
    _P_STATIC _P_INLINE _P_CONSTEXPR glm::vec4 red = {1.f, 0.f, 0.f, 1.f};
    _P_STATIC _P_INLINE _P_CONSTEXPR glm::vec4 blue = {0.f, 0.f, 1.f, 1.f};
    _P_STATIC _P_INLINE _P_CONSTEXPR glm::vec4 green = {0.f, 1.f, 0.f, 1.f};
    _P_STATIC _P_INLINE _P_CONSTEXPR glm::vec4 black = {0.f, 0.f, 0.f, 0.f};
};

struct TstVertex
{
    glm::vec3 position={0.f, 0.f, 0.f};
    glm::vec3 normal={0.f, 0.f, -1.f};
    glm::vec4 color={1.f, 1.f, 1.f, 1.f};
    glm::vec2 uv{0.f, 0.f};

    float id=0.f;
};

inline std::array<TstVertex, 4> quad_vertex(float width=1, float height=1, glm::vec4 color=Color::white, int id=0)
{
    // vertex 1
    TstVertex v1;
    v1.position = {-width / 2.f, height / 2.f, 0.f};
    v1.color = color;
    v1.uv = {0.f, 1.f};

    // vertex 2
    TstVertex v2;
    v2.position = {width / 2.f, height / 2.f, 0.f};
    v2.color = color;
    v2.uv = {1.f, 1.f};

    // vertex 3
    TstVertex v3;
    v3.position = {width / 2.f, - height / 2.f, 0.f};
    v3.color = color;
    v3.uv = {1.f, 0.f};

    // vertex 4
    TstVertex v4;
    v4.position = {-width / 2.f, -height / 2.f, 0.f};
    v4.color = color;
    v4.uv = {0.f, 0.f};

    return {v1, v2, v3, v4};
}

class SandboxLevel : Pong::AbstractLevel {
private:
    TstVertex* const m_vertex_data = new TstVertex[max_quads * 4];

    unsigned int m_quadVA;
    unsigned int m_quadVB;
    unsigned int m_quadIB;

protected:
    void _level_setup() override
    {
        uint32_t quads_num = 2;
        // remember remove vertex_data
        auto* ptr = m_vertex_data;
        // get quads
        for (uint32_t i=0; i<std::min(quads_num, max_quads); i++)
        {
            for (auto& vert: quad_vertex())
            {
                memcpy(ptr++, &vert, sizeof(TstVertex));
            }
        }

        glCreateVertexArrays(1, &m_quadVA);

        glBindVertexArray(m_quadVA);
        glCreateBuffers(1, &m_quadVB);
        glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TstVertex) * max_quads * 4, nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexArrayAttrib(m_quadVB, 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TstVertex), (const void*)offsetof(TstVertex, position));

        glEnableVertexArrayAttrib(m_quadVB, 1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TstVertex), (const void*)offsetof(TstVertex, normal));

        glEnableVertexArrayAttrib(m_quadVB, 2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TstVertex), (const void*)offsetof(TstVertex, color));

        glEnableVertexArrayAttrib(m_quadVB, 3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TstVertex), (const void*)offsetof(TstVertex, uv));

        glEnableVertexArrayAttrib(m_quadVB, 4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(TstVertex), (const void*)offsetof(TstVertex, id));

        uint32_t indices[] = {
                0,1,2,2,3,0,
                4,5,6,6,7,4
        };

        glCreateBuffers(1, &m_quadIB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadIB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    }
};

unsigned int CompileShaderD(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type==GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            " shader!\n";

        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, vertexShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderProgramSource ParseShadert(const std::string& file_path)
{
    std::ifstream stream(file_path);

    enum class ShaderType
    {
        NONE=-1, VERTEX=0, FRAGMENT=1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                // set mode to vertex
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                // mode to fragment
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

#endif //PONG_SANDBOX_H
