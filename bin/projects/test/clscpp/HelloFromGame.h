#pragma once
#include <GarinCore.h>

class HelloFromGame : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};
REGISTER_GAMEBEHAVIOUR_CLASS(HelloFromGame)