#pragma once
#include <GarinNatives.h>
#include <GarinGraphics.h>
#include <GarinMaths.h>
#include <GarinEvents.h>
#include <GarinComponents.h>

class ExampleClass : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};
