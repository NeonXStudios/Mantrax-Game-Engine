#pragma once
#include <iostream>
#include <FileManager.h>
#include <Core.h>

class GARINLIBS_API EditorConfigs
{
public:
    std::string current_scene = "New Scene";
    std::string start_scene = "New Scene";
    std::string current_proyect = "projects/test/";

    void save_config();
    void load_config();
};
