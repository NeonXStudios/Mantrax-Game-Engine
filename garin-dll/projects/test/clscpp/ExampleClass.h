#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinGraphics.h>

class ExampleClass : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};

// DEFITION FOR CREATE CLASS
GCLASS(ExampleClass)