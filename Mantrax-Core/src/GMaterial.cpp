#include "../includes/GMaterial.h"
#include <ResourcesManager.h>

void GMaterial::defines()
{
    GVAR(VertexPath, "shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentPath, "shaders/mesh_fragment.glsl", std::string);
}

void GMaterial::init()
{
    std::string vertexpath = FileManager::get_project_path() + GETVAR(VertexPath, std::string);
    std::string fragpath = FileManager::get_project_path() + GETVAR(FragmentPath, std::string);
    p_shader = new Shader(vertexpath.c_str(), fragpath.c_str());
}

void GMaterial::update()
{
}

void GMaterial::clean()
{
}
