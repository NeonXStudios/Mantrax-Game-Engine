#pragma once

#include <iostream>
#include <GameBehaviour.h>

class GameBehaviourRunner
{
public:
    std::vector<std::shared_ptr<GameBehaviour>> behaviours;

    void on_init();
    void on_tick();
};