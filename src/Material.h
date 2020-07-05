//#include "pongTypes.h"

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader_s.h"

#include <iostream>
#include <utility>
#include <vector>
#include <map>

namespace Pong {

    class Texture
    {
    protected:
        std::string _name;
        unsigned int _texture_id{};
        std::string _path;
        std::string _texture_type;
    public:

        static unsigned int load_texture(char const *path, const bool &gammaCorrection);

        explicit Texture(std::string name): _name(std::move(name)){}
        Texture(std::string name, const std::string& path, std::string texture_type);

        virtual ~Texture();

        [[nodiscard]] unsigned int get_id() const { return _texture_id; }
        std::string get_path() { return _path; }
        std::string get_texture_type() { return _texture_type; }
        std::string get_name() { return _name; }
    };

    class SkyBox : public Texture
    {

        std::vector<std::string> _cubemap_textures;
    public:
        SkyBox(std::string name,
               const std::string &right, const std::string &left, const std::string &top,
               const std::string &bottom, const std::string& front, const std::string& back) :
                Texture(std::move(name))
        {
            _cubemap_textures.push_back(right);
            _cubemap_textures.push_back(left);
            _cubemap_textures.push_back(top);
            _cubemap_textures.push_back(bottom);
            _cubemap_textures.push_back(back);

            _texture_id = load_cubemap(_cubemap_textures);
        }

        static unsigned int load_cubemap(const std::vector<std::string>& faces);

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

        std::string get_name()
        {
            return _name;
        }

        virtual void use();

        virtual void end_use();
        /// unbind textures
    };
}
#endif // !MATERIAL_H