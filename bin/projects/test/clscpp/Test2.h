#pragma once
#include <GarinCore.h>

class Test2 : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};

REGISTER_GAMEBEHAVIOUR_CLASS(Test2)