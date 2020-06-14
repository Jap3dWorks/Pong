//#pragma once
#ifndef MESH_H
#define MESH_H

#include "Shader_s.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    //texCoords
    glm::vec2 TexCoords;
    //tangent
    glm::vec3 Tangent;
    //biTangent
    glm::vec3 Bitangent;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector <Texture> Texture;
    unsigned int VAO;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> Texture):
        vertices(vertices), indices(indices), Texture(Texture)
    {
        setupMesh();
    }

    void Draw(Shader shader)
    {
        // bind appropiate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heighNr = 1;
        for(unsigned int i=0; i < Texture.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            // retrieve texture number
            // name convention texture_diffuse1, texture_diffuse2, ...
            std::string number;
            std::string name = Texture[i].type;
            if (name == "texture_diffuse"){
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular"){
                number = std::to_string(specularNr++);
            }
            else if (name == "texture_normal"){
                number = std::to_string(normalNr++);
            }
            else if (name == "texture_height"){
                number = std::to_string(heighNr++);
            }

            // set the uniform shader value
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // bind texture
            glBindTexture(GL_TEXTURE_2D, Texture[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // no affect more to VAO

        // set everething back to default once configures
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data
    unsigned int VBO, EBO;

    // initializes the buffers;
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            &vertices[0],
            GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            &indices[0],
            GL_STATIC_DRAW);

        // set the vertexattribute pointers
        // vertex points
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, Normal));

        // vertextexture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, TexCoords));

        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, Tangent));

        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};

#endif // !MESH_H
