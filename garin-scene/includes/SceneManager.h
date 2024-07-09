#pragma once
#include <iostream>
#include <GarinComponents.h>
#include "Scene.h"

class Scene;
class SceneManager
{
public:
    Scene *OpenScene;
    static SceneManager *instance;
    static void create();
    static void release();

    static SceneManager *GetSceneManager();
    static Scene *GetOpenScene();
    static string *GetOpenSceneName();
    static void LoadScene(string scenePath, string sceneName);
    Entity *NewEntity();
    Entity *NewEntityNonSetupScene();
    Entity *GetObjectByID(int id);
    Entity *GetObjectPerIndex(int index);
    Entity *CloneEntity(Entity *entity);
    Entity *Destroy(Entity *object);
    void ClearOpenScene();
};