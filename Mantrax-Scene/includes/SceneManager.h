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
    std::vector<Scene *> opened_scenes = std::vector<Scene *>();
    PhysicsEngine *physic_world;
    bool loading_new_scene;

    glm::vec3 CalculateLocalPosition(const glm::vec3 &globalPos, const glm::quat &parentRot, const glm::vec3 &parentScale);
    glm::quat CalculateLocalRotation(const glm::quat &globalRot, const glm::quat &parentRot);
    glm::vec3 CalculateLocalScale(const glm::vec3 &globalScale, const glm::vec3 &parentScale);
    Scene* get_parent_scene_from_object(Entity* object);

    string *get_open_scene_name();
    Scene *get_current_scene();
    void start_physic_world();
    void link_scene(Scene *scene_to_link);
    Scene *make_new_empty_scene(std::string scene_name);
    void close_scene(Scene* p_scene);
    
    void on_awake();
    void on_start();
    void on_update();
    void on_edition_mode();
    void on_clean_scenes();
    
    Scene* load_scene(std::string scene_name_new, bool is_additive = false, std::string extension = ".scene");
    void clear_open_scene();
    static void load_scene_wrapped(std::string scene_name_new, bool is_additive = false);
};