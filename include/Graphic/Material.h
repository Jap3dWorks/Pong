#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Core/Render.h"

#include <iostream>
#include <utility>
#include <vector>
#include <map>

namespace Pong
{
    class Scene;
    class Render;
}
namespace Pong {

    class Texture
    {
    protected:
        std::string _name;
        unsigned int _texture_id{};
        std::string _path;
        std::string _texture_type;
        unsigned int _gl_bind_type = GL_TEXTURE_2D;

    public:
        static unsigned int load_texture(const char *path, const bool &gammaCorrection);

        explicit Texture(std::string  name, std::string texture_type):
        _name(std::move(name)), _texture_type(std::move(texture_type))
        {}

        Texture(std::string name, const std::string& path, std::string texture_type);

        virtual ~Texture();

        _NODISCARD unsigned int get_id() const { return _texture_id; }
        _NODISCARD unsigned int get_gl_bind_type() const {return _gl_bind_type;}
        std::string get_path() { return _path; }
        std::string get_texture_type() { return _texture_type; }
        std::string get_name() { return _name; }
    };

    class SkyBox : public Texture
    {
        std::vector<std::string> _cubemap_textures;

    protected:

    public:
        SkyBox(std::string name, std::string texture_type,
               const std::string &right, const std::string &left, const std::string &top,
               const std::string &bottom, const std::string &front, const std::string &back) :
                Texture(std::move(name), std::move(texture_type))
                {
            _cubemap_textures.push_back(right);
            _cubemap_textures.push_back(left);
            _cubemap_textures.push_back(top);
            _cubemap_textures.push_back(bottom);
            _cubemap_textures.push_back(front);
            _cubemap_textures.push_back(back);

            _texture_id = load_skybox_textures(_cubemap_textures);
            Texture::_gl_bind_type = GL_TEXTURE_CUBE_MAP;
        }

        static unsigned int load_skybox_textures(const std::vector<std::string>& faces);

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
        unsigned int order{50};

        Material(std::string name, Shader* shader, std::vector<Texture*> textures);

        virtual ~Material();

        _NODISCARD Shader* get_shader() const;

        // stared Parameters that not vary each frame,
        // it can be a roughness value.
        void update_params();

        void set_param(const std::string&, float);

        void set_param(const std::string&, glm::vec3);

        void set_param(const std::string&, int);

        void set_param(const std::string&, glm::mat4);

        virtual void update_shader(const Render *render, const Scene *scene);

        [[nodiscard]]std::string get_name() const { return _name; }

        virtual void use();

        /// unbind Textures
        virtual void end_use();
    };

    /**Overrides update_shader from Material class*/
    class SKyBoxMaterial: public Material
    {
    public:
        SKyBoxMaterial(std::string name, Shader* shader, std::vector<Texture*> textures):
            Material(std::move(name), shader, std::move(textures) ){}

        void update_shader(const Render *render, const Scene *scene) override;
    };
}

#endif // !MATERIAL_H