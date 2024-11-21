#pragma once
#define NO_INCLUDE_GRAPHICS_CLASS
#include <GarinBehaviours.h>
#include <GarinNatives.h>
#include <GarinGraphics.h>
#include <ECS.h>
#include <camera.h>
#include <PhysicsEngine.h>

#include <GLFW/glfw3.h>
#include <Core.h>

using namespace std;

class Camera;

class GARINLIBS_API Scene
{
public:
    string scene_name = "New Scene";
    Camera *main_camera;

    vector<Entity *> objects_worlds = vector<Entity *>();
    PhysicsEngine *physic_world;
    bool unload_scene;

    // SCENE VIRTUALS
    virtual void on_awake() {};
    virtual void on_start() {};
    virtual void on_update(float delta_time) {};
    virtual void on_draw() {};
    virtual void on_destroy() {};
    virtual void draw_ui() {};
    virtual void on_edition_mode(float delta_time) {}

    // SCENE FUNCTIONS
    void awake();
    void init();
    void update(float delta_time);

    // FUNCTION ENTITYS
    Entity *make_entity();
    Entity *get_entity_by_index(int index);
    Entity *get_entity_by_id(int id);
    Entity *get_entity_by_id_string(std::string id);
    Entity *get_entity_by_name(string index);

    void destroy(Entity *object_to_destroy);

    std::string demangle(const char *mangled_name)
    {
        std::string demangled_name = mangled_name;

        if (demangled_name.find("class ") == 0)
        {
            demangled_name = demangled_name.substr(6);
        }

        return demangled_name;
    }
};