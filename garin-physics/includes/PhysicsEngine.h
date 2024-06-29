#pragma once

#include <PxPhysicsAPI.h>
#include "GarinCollisionEvents.h"

class PhysicsEngine
{
public:
    physx::PxDefaultAllocator mDefaultAllocatorCallback;
    physx::PxDefaultErrorCallback mDefaultErrorCallback;
    physx::PxDefaultCpuDispatcher *mDispatcher = NULL;
    physx::PxTolerancesScale mToleranceScale;

    physx::PxFoundation *mFoundation = NULL;
    physx::PxPhysics *mPhysics = NULL;

    physx::PxScene *mScene = NULL;
    physx::PxMaterial *mMaterial = NULL;

    physx::PxPvd *mPvd = NULL;

    void start_world_physics();
    void update_world_physics();
};