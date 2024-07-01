#include <PxPhysicsAPI.h>
#include <iostream>

using namespace physx;

class GarinCollisionEvents : public PxSimulationEventCallback
{
public:
    void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) override
    {
        for (PxU32 i = 0; i < nbPairs; i++)
        {
            const PxContactPair &cp = pairs[i];
            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                PxActor *actor0 = pairHeader.actors[0];
                PxActor *actor1 = pairHeader.actors[1];
                // Do something with the colliding actors
                std::cout << "Collision detected between " << actor0->getName() << " and " << actor1->getName() << std::endl;
            }
        }
    }

    void onTrigger(PxTriggerPair *pairs, PxU32 count) override
    {
        for (PxU32 i = 0; i < count; i++)
        {
            PxTriggerPair &triggerPair = pairs[i];
            PxActor *triggerActor = triggerPair.triggerActor;
            PxActor *otherActor = triggerPair.otherActor;
            bool isEnter = triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND;
            bool isExit = triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_LOST;

            if (isEnter)
            {
                std::cout << "Trigger entered: " << triggerActor->getName() << " triggered by " << otherActor->getName() << std::endl;
            }
            else if (isExit)
            {
                std::cout << "Trigger exited: " << triggerActor->getName() << " triggered by " << otherActor->getName() << std::endl;
            }
        }
    }

    void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) override
    {
        for (PxU32 i = 0; i < count; i++)
        {
            // Implementación para manejar rupturas de constraints
            PxConstraintInfo &constraint = constraints[i];
            std::cout << "Constraint broken: " << constraint.constraint << std::endl;
        }
    }

    void onWake(PxActor **actors, PxU32 count) override
    {
        for (PxU32 i = 0; i < count; i++)
        {
            // Implementación para manejar eventos de wake
            PxActor *actor = actors[i];
            std::cout << "Actor woke up: " << actor->getName() << std::endl;
        }
    }

    void onSleep(PxActor **actors, PxU32 count) override
    {
        for (PxU32 i = 0; i < count; i++)
        {
            // Implementación para manejar eventos de sleep
            PxActor *actor = actors[i];
            std::cout << "Actor went to sleep: " << actor->getName() << std::endl;
        }
    }

    void onAdvance(const PxRigidBody*const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {}
};
