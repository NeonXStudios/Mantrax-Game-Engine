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

class Scene
{
public:
    string scene_name = "New Scene";
    Camera *main_camera;

    vector<Entity *> objects_worlds = vector<Entity *>();
    PhysicsEngine *physic_world;
    bool unload_scene;

    // SCENE VIRTUALS
    virtual void on_start() {};
    virtual void on_update(float delta_time) {};
    virtual void on_draw() {};
    virtual void on_destroy() {};
    virtual void draw_ui() {};
    virtual void on_edition_mode(float delta_time) {}

    // SCENE FUNCTIONS
    void init();
    void update(float delta_time);

    void load_scene(std::string scene_name);
    void save_scene();

    // FUNCTION ENTITYS
    Entity *make_entity();
    Entity *get_entity_by_index(int index);
    Entity *get_entity_by_id(int id);
    Entity *get_entity_by_id_string(std::string id);
    Entity *get_entity_by_name(string index);

    void destroy(Entity *object_to_destroy);

    std::string demangle(const char *mangled_name)
    {
        // Implementación básica para fines ilustrativos

        // Copia el nombre mangled para trabajar sobre él
        std::string demangled_name = mangled_name;

        // Aquí puedes aplicar las reglas específicas de MSVC para demangle el nombre
        // Por ejemplo, remover prefijos, sufijos, convertir códigos mangled, etc.

        // Implementación básica: remover prefijo de clase
        if (demangled_name.find("class ") == 0)
        {
            demangled_name = demangled_name.substr(6); // Remueve "class "
        }

        // Puedes agregar más lógica según tus necesidades

        return demangled_name;
    }
};