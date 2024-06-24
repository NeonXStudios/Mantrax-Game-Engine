#pragma once
#include <ECS.h>
#include <iostream>
#include <string>

using namespace std;

class GameBehaviour
{
public:
    virtual void on_init() {}
    virtual void on_tick() {}

    Entity *find_object_by_name(string name);
    Entity *find_object_by_index(int index);
};
