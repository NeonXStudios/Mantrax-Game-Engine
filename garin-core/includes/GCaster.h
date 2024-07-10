#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"

using namespace physx;
using namespace glm;

struct GARINLIBS_API GCastHit
{
    vec3 point;
    vec3 normal;
    Entity *entity;
};

class GARINLIBS_API GCaster
{
public:
    bool RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Lenght, GCastHit *hit, PxHitFlags hitFlags = PxHitFlags(PxHitFlag::eDEFAULT), PxU32 layerMask = LAYER_0);
};