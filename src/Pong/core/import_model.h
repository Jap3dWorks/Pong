#ifndef GL_TEST_IMPORT_MODEL_H
#define GL_TEST_IMPORT_MODEL_H

#include "geometry_data.h"
#include "Pong/core/actor/actor.h"

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

// https://learnopengl.com/Model-Loading/Mesh

namespace pong {
    /**
     * Import model from a file like fbx or obj, the shapes that compose the model,
     * Will be added to the Map.
     * Returns: Number of shapes imported.
     * */
    using MeshVector = std::vector<Mesh>;

    static inline auto _process_mesh(const aiMesh &mesh) -> Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh r_mesh;

        for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
            Vertex vertex{};
            glm::vec3 vector;
            // positions
            vector.x = mesh.mVertices[i].x;
            vector.y = mesh.mVertices[i].y;
            vector.z = mesh.mVertices[i].z;
            vertex.position = vector;
            //normals
            vector.x = mesh.mNormals[i].x;
            vector.y = mesh.mNormals[i].y;
            vector.z = mesh.mNormals[i].z;
            vertex.normal = vector;

            // texture coordinates
            if (mesh.mTextureCoords[0])  // only one id
            {
                glm::vec2 vec;
                vec.x = mesh.mTextureCoords[0][i].x;
                vec.y = mesh.mTextureCoords[0][i].y;
                vertex.tex_coords = vec;
            } else vertex.tex_coords = glm::vec2(0.f);

            // tangent
//            vector.x = mesh.mTangents[i].x;
//            vector.y = mesh.mTangents[i].y;
//            vector.z = mesh.mTangents[i].z;
//            vertex.tangent = vector;

            // bitangent
//            vector.x = mesh.mBitangents[i].x;
//            vector.y = mesh.mBitangents[i].y;
//            vector.z = mesh.mBitangents[i].z;
//            vertex.bitangent = vector;

            r_mesh.vertices.push_back(vertex);
        }

        // vertex _indices
        for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
            aiFace face = mesh.mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                r_mesh.indices.push_back(face.mIndices[j]);
            }
        }

        return r_mesh;
    }

    inline void _process_node(aiNode *node,
                       const aiScene *&scene,
                       MeshVector &out_result) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            const aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
            out_result.push_back(_process_mesh(*ai_mesh));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            _process_node(node->mChildren[i], scene, out_result);
        }
    }


    inline auto import_model(const std::string &model_path) -> MeshVector {

        // load file
        Assimp::Importer importer;
        const aiScene *ai_scene = importer.ReadFile(
                model_path,
                aiProcess_Triangulate | aiProcess_CalcTangentSpace);

        // check for errors
        if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode) {
            LOG_ERROR("ASSIMP:: " << importer.GetErrorString());
            throw MeshException(importer.GetErrorString());
        }

        // directory path
        std::string directory = model_path.substr(0,
                                                  model_path.find_last_of('/'));

        std::string file_name = model_path.substr(model_path.find_last_of('/') + 1);

        MeshVector result;
        _process_node(ai_scene->mRootNode, ai_scene, result);

//        for(auto shape: result)
//        {
//            shape->set_name(file_name + ":" + shape->get_name());
//            shape_map[shape->get_name()] = shape;
//            assign_shape(shape, actor);
//        }

        return result;
    }
}

#endif //GL_TEST_IMPORT_MODEL_H
