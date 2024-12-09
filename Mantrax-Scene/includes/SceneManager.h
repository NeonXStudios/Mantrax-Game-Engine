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

    static SceneManager *GetSceneManager();
    static Scene *GetOpenScene();
    static string *GetOpenSceneName();
    static void start_physic_world();
    static Scene *get_current_scene();
    static void load_scene(std::string scene_name_new, bool is_additive = false);
    static void link_scene(Scene *scene_to_link);

    static void on_awake();
    static void on_start();
    static void on_update();
    static void on_edition_mode();
    static void on_clean_scenes();

    Entity *NewEntity();
    Entity *NewEntityNonSetupScene();
    Entity *GetObjectByID(int id);
    Entity *GetObjectPerIndex(int index);
    Entity *GetObjectPerTag(std::string tag);
    Entity *GetObjectPerName(std::string name);
    Entity *CloneEntity(Entity *entity);
    Entity *Destroy(Entity *object);
    void ClearOpenScene();
};