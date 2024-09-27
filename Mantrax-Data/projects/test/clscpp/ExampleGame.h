#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>

class ExampleGame : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};
