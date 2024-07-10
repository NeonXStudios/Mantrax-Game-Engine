#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"

class GARINLIBS_API GCollision : public Component
{
public:
    glm::vec3 boxSize = glm::vec3(1, 1, 1);
    bool is_trigger;

    physx::PxShape *shape = NULL;
    PxU32 layerMask0 = 0;
    PxU32 layerMask1 = 0;
    PxU32 layerMask2 = 0;
    PxU32 layerMask3 = 0;

    void defines();
    void init() override;
    void update() override;
    void setLayerMask(PxU32 mask0, PxU32 mask1 = 0, PxU32 mask2 = 0, PxU32 mask3 = 0)
    {
        layerMask0 = mask0;
        layerMask1 = mask1;
        layerMask2 = mask2;
        layerMask3 = mask3;
    }
};
