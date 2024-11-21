#pragma once
#include <GarinNatives.h>
#include <ECS.h>
#include <GarinBehaviours.h>
#include "Core.h"
#include <PerlinGenerator.h>

class GARINLIBS_API GNoise : public Component
{
public:
    PerlinGenerator *perlin;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};
