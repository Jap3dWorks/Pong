//#include "pongTypes.h"

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader_s.h"

#include <iostream>
#include <vector>
#include <map>

namespace Pong {

    class Texture
    {
        std::string _name;
        int _texture_id;
        std::string _path;
        std::string _texture_type;
    public:
        Texture(std::string name, std::string path, std::string texture_type);

        virtual ~Texture();

        int get_id() { return _texture_id; }
        std::string get_path() { return _path; }
        std::string get_texture_type() { return _texture_type; }
        std::string get_name() { return _name; }
    };

    class Material
    {
    private:
        bool _is_setup = false;

        void _setup_material();

        std::map<std::string, float> _float_params;
        std::map<std::string, int> _int_params;
        std::map<std::string, glm::vec3> _vec3_params;
        std::map<std::string, glm::mat4> _mat4_params;

    protected:
        Shader* _shader;
        std::vector<Texture*> _textures;
        std::string _name;

    public:
        Material(std::string name, Shader* shader, std::vector<Texture*> textures);

        virtual ~Material();

        Shader* get_shader();

        void set_param(std::string, float);
        void set_param(std::string, glm::vec3);
        void set_param(std::string, int);
        void set_param(std::string, glm::mat4);

        virtual void use();

        virtual void end_use();
        /// unbind textures
    };
}
#endif // !MATERIAL_H