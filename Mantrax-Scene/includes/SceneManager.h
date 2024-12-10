#pragma once
#include <iostream>
#include <Scene.h>
#include <Core.h>
#include <GarinComponents.h>
#include <PhysicsEngine.h>

class Scene;
class GARINLIBS_API SceneManager
{
public:
    static SceneManager *instance;

    std::vector<Scene *> opened_scenes = std::vector<Scene *>();
    PhysicsEngine *physic_world;

    static void create();
    static void release();

    static SceneManager *get_scene_manager();
    static string *get_open_scene_name();
    static Scene *get_open_scene();
    static Scene *get_current_scene();
    static void start_physic_world();
    static void load_scene(std::string scene_name_new, bool is_additive = false);
    static void link_scene(Scene *scene_to_link);

    static void on_awake();
    static void on_start();
    static void on_update();
    static void on_edition_mode();
    static void on_clean_scenes();

    void clear_open_scene();
};