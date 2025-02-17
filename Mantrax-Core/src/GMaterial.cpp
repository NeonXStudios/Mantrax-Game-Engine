#include "../includes/GMaterial.h"
#include <ResourcesManager.h>
#include <RenderPipeline.h>

void GMaterial::defines()
{
    GVAR(VertexPath, "assets/shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentPath, "assets/shaders/mesh_fragment.glsl", std::string);
}

void GMaterial::init()
{
    std::string vertexpath = FileManager::get_project_path() + GETVAR(VertexPath, std::string);
    std::string fragpath = FileManager::get_project_path() + GETVAR(FragmentPath, std::string);
    p_shader = new Shader(vertexpath.c_str(), fragpath.c_str());
}

void GMaterial::update()
{
    for (const auto &[name, texture] : textures)
    {
        texture->use_texture(p_shader->ID);
    }
}

void GMaterial::clean()
{
}

GMaterial::GMaterial()
{
    RenderPipeline::register_new_material(this);
}

GMaterial::~GMaterial()
{
    RenderPipeline::unregister_material(this);
}