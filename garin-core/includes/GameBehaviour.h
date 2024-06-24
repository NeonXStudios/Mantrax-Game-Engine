#ifndef GAMEBEHAVIOUR_H
#define GAMEBEHAVIOUR_H
#include <ECS.h>

class GameBehaviour
{
public:
    virtual void on_init() {}
    virtual void on_tick() {}

    Entity *find_object_by_name(string name);
    Entity *find_object_by_index(int index);
};
#endif // GAMEBEHAVIOUR_H