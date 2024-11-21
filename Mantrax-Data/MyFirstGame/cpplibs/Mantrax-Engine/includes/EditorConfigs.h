#pragma once
#include <iostream>
#include <FileManager.h>
#include <Core.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib") 

class GARINLIBS_API EditorConfigs
{
public:
    std::string current_scene = "New Scene";
    std::string start_scene = "New Scene";
    std::string current_proyect = "--Null--/--Null--/";
    std::string current_theme = "DefaultWhite";
    float camera_speed_sens = 5.0f;

    bool project_select = false;

    void save_config();
    void load_config();
};
