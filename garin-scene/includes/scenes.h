#pragma once
#define WIN32_LEAN_AND_MEAN
#include <ECS.h>
#include <camera.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include <FileManager.h>
#include <nlohmann/json.hpp>
#include <typeinfo>
#include <cxxabi.h>
#include <random>
#include <iomanip>

using namespace std;

class scenes
{
public:
    string scene_name = "New Scene";
    Camera *main_camera;

    vector<Entity *> objects_worlds = vector<Entity *>();

    // SCENE VIRTUALS
    virtual void on_start() {};
    virtual void on_update(float delta_time) {};
    virtual void on_draw() {};
    virtual void on_destroy() {};
    virtual void draw_ui() {};

    // SCENE FUNCTIONS
    void load_scene(std::string scene_name);
    void save_scene();

    // FUNCTION ENTITYS
    Entity *make_entity();
    Entity *get_entity_by_index(int index);
    Entity *get_entity_by_id(int id);
    Entity *get_entity_by_name(string index);

    std::string demangle(const char *name)
    {
        int status = -1;
        std::unique_ptr<char, void (*)(void *)> res{
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
            std::free};
        return (status == 0) ? res.get() : name;
    }
};