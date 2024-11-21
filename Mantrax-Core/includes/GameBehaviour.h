#pragma once

#include <ECS.h>
#include <iostream>
#include <string>
#include "Core.h"

using namespace std;

class GARINLIBS_API GameBehaviour
{
public:
    Entity *self;

    virtual void on_awake() {}
    virtual void on_init() {}
    virtual void on_tick() {}
    virtual void on_gui_draw() {}

    Entity *find_object_by_name(string name);
    Entity *find_object_by_index(int index);
};
