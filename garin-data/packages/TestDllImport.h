#pragma once

#include <GarinComponents.h>

class TestDllImport : public GameBehaviour
{
public:
    void on_init() override;
    void on_tick() override;
};
