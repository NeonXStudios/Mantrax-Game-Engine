#include <GarinCollisionEvents.h>
#include <SceneManager.h>

using namespace physx;

void GarinCollisionEvents::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair &cp = pairs[i];

        if (!cp.shapes[0] || !cp.shapes[1])
            continue;

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            PxActor *actor0 = pairHeader.actors[0];
            PxActor *actor1 = pairHeader.actors[1];

            if (!actor0 || !actor1)
                continue;

            std::cout << "Collision detected between " << (actor0->getName() ? actor0->getName() : "unnamed") << " and " << (actor1->getName() ? actor1->getName() : "unnamed") << std::endl;

            bool isTrigger0 = cp.shapes[0]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE;
            bool isTrigger1 = cp.shapes[1]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE;

            if (isTrigger0 || isTrigger1)
            {
                std::cout << "Trigger interaction detected!" << std::endl;
            }
        }
    }
}

void GarinCollisionEvents::onTrigger(PxTriggerPair *pairs, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        PxTriggerPair &triggerPair = pairs[i];
        PxActor *triggerActor = triggerPair.triggerActor;
        PxActor *otherActor = triggerPair.otherActor;
        bool isEnter = triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND;
        bool isExit = triggerPair.status == PxPairFlag::eNOTIFY_TOUCH_LOST;

        if (isEnter || isExit)
        {
            bool entering = isEnter;

            int otherActorId, triggerActorId;
            try
            {
                otherActorId = std::stoi(otherActor->getName());
                triggerActorId = std::stoi(triggerActor->getName());
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error al convertir el nombre a ID: " << e.what() << std::endl;
                return;
            }

            Entity *entity = SceneManager::get_current_scene()->find_entity_by_id(otherActorId);
            Entity *entity_me = SceneManager::get_current_scene()->find_entity_by_id(triggerActorId);

            if (!entity || !entity_me)
            {
                std::cerr << "Entidad no encontrada." << std::endl;
                return;
            }

            if (entity->hasComponent<GScript>() && entity_me->hasComponent<GScript>())
            {
                auto &script = entity_me->getComponent<GScript>();
                if (entity->hasComponent<GCollision>() && script.behaviour)
                {
                    GCollision *collision = &entity->getComponent<GCollision>();
                    if (entering)
                    {
                        script.behaviour->on_trigger_enter(collision);
                    }
                    else
                    {
                        script.behaviour->on_trigger_exit(collision);
                    }
                }
            }
        }
    }
}

void GarinCollisionEvents::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        PxConstraintInfo &constraint = constraints[i];
        std::cout << "Constraint broken: " << constraint.constraint << std::endl;
    }
}

void GarinCollisionEvents::onWake(PxActor **actors, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        PxActor *actor = actors[i];
        std::cout << "Actor woke up: " << actor->getName() << std::endl;
    }
}

void GarinCollisionEvents::onSleep(PxActor **actors, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        PxActor *actor = actors[i];
        std::cout << "Actor went to sleep: " << actor->getName() << std::endl;
    }
}

void GarinCollisionEvents::onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) {}