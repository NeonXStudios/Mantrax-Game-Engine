#pragma once
#include "../EngineUIBehaviour.h"

struct Data
{
    int id = -1;
    std::string name_material = "New Material";
    std::string main_path = "assets/packages/textures/grid.png";
    std::string normal_path = "assets/packages/textures/grid.png";
    std::string height_path = "assets/packages/textures/grid.png";
    std::string fragment_path = "assets/shaders/mesh_fragment.glsl";
    std::string vertex_path = "assets/shaders/mesh_vertex.glsl";
};

class MaterialMaker : public EngineUIBehaviour
{
public:
    Data mat_struct = Data();
    bool opened_menu_to_save;
    void on_draw() override;
};
