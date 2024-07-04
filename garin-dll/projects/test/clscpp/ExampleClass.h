#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinMaths.h>

class ExampleClass : public GameBehaviour
{
public:
    float move_speed = 3.0f;
    float interpolationSpeed = 0.1f;
    glm::vec3 direction;

    void on_init() override;
    void on_tick() override;
};

// DEFITION FOR CREATE CLASS
GCLASS(ExampleClass)