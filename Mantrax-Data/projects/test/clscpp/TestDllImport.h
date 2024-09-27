#pragma once
#include <GarinNatives.h>
#include <GarinGraphics.h>
#include <GarinMaths.h>
#include <GarinComponents.h>

class TestDllImport : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};
