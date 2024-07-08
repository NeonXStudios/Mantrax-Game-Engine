#pragma once
#include <iostream>
#include <scenes.h>

class SceneManager
{
public:
    static scenes *current_scene;

    static void load_scene(std::string scene_name);
    static std::string get_name_scene();
};