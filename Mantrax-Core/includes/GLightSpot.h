#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"
#include "LayerManager.h"
#include <LightData.h>

class GLightSpot : public Component {
public:
    SpotLightData* light;
    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};