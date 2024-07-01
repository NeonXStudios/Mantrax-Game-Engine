#include "../includes/GMaterial.h"

void GMaterial::defines()
{
    GVAR(VertexPath, "assets/shaders/mesh_vertex.glsl", std::string);
    GVAR(FragmentPath, "assets/shaders/mesh_fragment.glsl", std::string);
}

void GMaterial::init()
{
    p_shader = new Shader(GETVAR(VertexPath, std::string).c_str(), GETVAR(FragmentPath, std::string).c_str());
}

void GMaterial::update()
{
}