#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"
#include "LayerManager.h"

class GARINLIBS_API GCollision : public Component
{
public:
    glm::vec3 boxSize = glm::vec3(1, 1, 1);
    bool is_trigger;

    physx::PxShape *shape = NULL;
    PxU32 layerMask0 = LAYER_1;
    bool lock_update;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};
