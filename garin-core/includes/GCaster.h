#pragma once
#include <GarinNatives.h>
#include <GarinComponents.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>

using namespace physx;
using namespace glm;

struct GCastHit
{
    vec3 point;
    vec3 normal;
    Entity *entity;
};

class GCaster
{
public:
    bool RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Lenght, GCastHit *hit, PxHitFlags hitFlags = PxHitFlags(PxHitFlag::eDEFAULT));
};