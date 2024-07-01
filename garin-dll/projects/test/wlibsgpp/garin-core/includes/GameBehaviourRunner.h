#pragma once

#include <iostream>
#include <GameBehaviour.h>
#include <Graphics.h>

class GameBehaviourRunner
{
public:
    std::vector<std::shared_ptr<GameBehaviour>> behaviours;

    void on_init()
    {
        // behaviours = GameBehaviourFactory::instance().create_all();

        // for (auto &behaviour : behaviours)
        // {
        //     if (behaviour)
        //     {
        //         behaviour->on_init();
        //     }
        // }
        // std::cout << "Registering behaviours" << std::endl;
    }

    void on_tick()
    {
        // for (auto &behaviour : behaviours)
        // {
        //     if (behaviour)
        //     {
        //         behaviour->on_tick();
        //     }
        // }
    }
};