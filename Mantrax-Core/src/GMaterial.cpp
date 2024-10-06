#include "../includes/GMaterial.h"

void GMaterial::defines()
{
    GVAR(VertexPath, "assets/shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentPath, "assets/shaders/mesh_fragment.glsl", std::string);
}

void GMaterial::init()
{
    CustomShader shader = CustomShaderParser::ParseShaderFile(FileManager::get_project_path() + GETVAR(SlabShader, std::string));
    shader.SaveToVariables();
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