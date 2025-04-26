#pragma once
#include "../EngineUIBehaviour.h"

struct Data
{
    int id = -1;
    std::string name_material = "New Material";
    std::string main_path = "assets/packages/textures/grid2.png";
    std::string normal_path = "assets/packages/textures/grid2.png";
    std::string height_path = "assets/packages/textures/grid2.png";
};

class MaterialMaker : public EngineUIBehaviour
{
public:
    Data mat_struct = Data();
    void on_draw() override;
};
