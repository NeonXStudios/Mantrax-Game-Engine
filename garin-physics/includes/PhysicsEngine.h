#pragma once

// #include <Jolt/Jolt.h>

// #include <Jolt/RegisterTypes.h>
// #include <Jolt/Core/Factory.h>
// #include <Jolt/Core/TempAllocator.h>
// #include <Jolt/Core/JobSystemThreadPool.h>
// #include <Jolt/Physics/PhysicsSettings.h>
// #include <Jolt/Physics/PhysicsSystem.h>
// #include <Jolt/Physics/Collision/Shape/BoxShape.h>
// #include <Jolt/Physics/Collision/Shape/SphereShape.h>
// #include <Jolt/Physics/Body/BodyCreationSettings.h>
// #include <Jolt/Physics/Body/BodyActivationListener.h>

#ifdef GARINENGINE
#define GARINENGINE_API __declspec(dllexport)
#else
#define GARINENGINE_API __declspec(dllimport)
#endif

#ifdef PX_PHYSX_COMMON_EXPORTS
#define PX_PHYSX_COMMON_API __declspec(dllexport)
#else
#define PX_PHYSX_COMMON_API __declspec(dllimport)
#endif

#include <PxPhysicsAPI.h>
#include <GarinNatives.h>

class PhysicsEngine
{
public:
    void start_world_physics();
    void update_world_physics();
};