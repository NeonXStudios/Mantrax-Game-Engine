#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"
#include "LayerManager.h"
#include <LightData.h>

class GLightDirectional : public Component {
public:
    DirectionalLightData* light;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};