#pragma once
#include <iostream>
#include <vector>
#include <ECS.h>
#include <camera.h>

using namespace std;

class scenes
{
public:
    Camera main_camera;

    vector<Entity *> objects_worlds = vector<Entity *>();

    virtual void on_start() = 0;
    virtual void on_update() = 0;
    virtual void on_destroy() = 0;

    // MAKE NEW ENTITY
    Entity *make_entity();

    // GET ENTITY BY INDEX
    Entity *get_entity_by_index(int index);
};