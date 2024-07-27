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
    static bool RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Lenght, GCastHit *hit, PxHitFlags hitFlags = PxHitFlags(PxHitFlag::eDEFAULT), PxU32 layerMask = LAYER_0);
    static bool LineCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit *hit, PxU32 layerMask);
};