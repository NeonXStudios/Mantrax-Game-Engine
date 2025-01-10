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
    static bool loading_new_scene;

    static void create();
    static void release();

    static glm::vec3 CalculateLocalPosition(const glm::vec3 &globalPos, const glm::quat &parentRot, const glm::vec3 &parentScale);
    static glm::quat CalculateLocalRotation(const glm::quat &globalRot, const glm::quat &parentRot);
    static glm::vec3 CalculateLocalScale(const glm::vec3 &globalScale, const glm::vec3 &parentScale);
    static Scene* get_parent_scene_from_object(Entity* object);

    static SceneManager *get_scene_manager();
    static string *get_open_scene_name();
    static Scene *get_current_scene();
    static void start_physic_world();
    static void load_scene_wrapped(std::string scene_name_new, bool is_additive = false);
    static void link_scene(Scene *scene_to_link);
    static Scene *make_new_empty_scene(std::string scene_name);

    static void on_awake();
    static void on_start();
    static void on_update();
    static void on_edition_mode();
    static void on_clean_scenes();

    void load_scene(std::string scene_name_new, bool is_additive = false);
    void clear_open_scene();
};