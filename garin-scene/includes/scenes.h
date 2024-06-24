#pragma once
#define NO_INCLUDE_GRAPHICS_CLASS
#include <GarinBehaviours.h>
#include <GarinNatives.h>
#include <GarinGraphics.h>
#include <GarinComponents.h>
#include <camera.h>
#include <PhysicsEngine.h>

#include <GLFW/glfw3.h>

using namespace std;

class scenes
{
public:
    string scene_name = "New Scene";
    Camera *main_camera;

    vector<Entity *> objects_worlds = vector<Entity *>();
    PhysicsEngine *physic_world;

    // SCENE VIRTUALS
    virtual void on_start() {};
    virtual void on_update(float delta_time) {};
    virtual void on_draw() {};
    virtual void on_destroy() {};
    virtual void draw_ui() {};

    // SCENE FUNCTIONS
    void init();
    void update(float delta_time);

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