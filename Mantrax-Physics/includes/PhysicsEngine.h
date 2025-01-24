#pragma once

#include <PxPhysicsAPI.h>
#include <GarinCollisionEvents.h>
#include <Core.h>
#include <GarinNatives.h>
#include <mutex>

class GARINLIBS_API PhysicsEngine
{
public:
    physx::PxDefaultAllocator mDefaultAllocatorCallback;
    physx::PxDefaultErrorCallback mDefaultErrorCallback;
    physx::PxDefaultCpuDispatcher *mDispatcher = NULL;
    physx::PxTolerancesScale mToleranceScale;
    PxControllerManager *gManager = nullptr;
    GarinCollisionEvents *collision_events;

    physx::PxFoundation *mFoundation = NULL;
    physx::PxPhysics *mPhysics = NULL;

    physx::PxScene *mScene = NULL;
    physx::PxMaterial *mMaterial = NULL;

    physx::PxPvd *mPvd = NULL;
    bool cleaning_components;

    static PxFilterFlags CustomFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags &pairFlags, const void *constantBlock, PxU32 constantBlockSize)
    {
        if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;
        }

        if (PxFilterObjectIsKinematic(attributes0) && PxFilterObjectIsKinematic(attributes1))
        {
            return PxFilterFlag::eSUPPRESS;
        }

        if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
        {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT |       // Habilitar contactos básicos
                        PxPairFlag::eSOLVE_CONTACT |         // Resolver contactos físicamente
                        PxPairFlag::eNOTIFY_TOUCH_FOUND |    // Notificar cuando se encuentra contacto
                        PxPairFlag::eNOTIFY_TOUCH_PERSISTS | // Notificar mientras el contacto persiste
                        PxPairFlag::eNOTIFY_TOUCH_LOST |     // Notificar cuando se pierde el contacto
                        PxPairFlag::eNOTIFY_CONTACT_POINTS;  // Proporcionar puntos de contacto

            return PxFilterFlag::eDEFAULT;
        }

        return PxFilterFlag::eSUPPRESS;
    }

    void start_world_physics();
    void update_world_physics();
    void delete_phys_world();
    void shutdown_physics(); 
    void clear_components_in_world();
};