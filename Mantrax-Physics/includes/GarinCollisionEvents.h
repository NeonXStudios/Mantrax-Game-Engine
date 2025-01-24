#pragma once
#include <PxPhysicsAPI.h>
#include <iostream>
#include <Core.h>

using namespace physx;

class GARINLIBS_API GarinCollisionEvents : public PxSimulationEventCallback
{
public:
    bool locked;

    void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) override;

    void onTrigger(PxTriggerPair *pairs, PxU32 count) override;

    void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) override;

    void onWake(PxActor **actors, PxU32 count) override;

    void onSleep(PxActor **actors, PxU32 count) override;

    void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) override;
};
