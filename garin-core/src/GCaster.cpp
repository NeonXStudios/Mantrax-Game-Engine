#include "../includes/GCaster.h"
#include <GarinGraphics.h>

bool GCaster::RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit *hitRay, PxHitFlags hitFlags)
{
    if (!hitRay)
    {
        std::cerr << "Error: hitRay is null." << std::endl;
        return false;
    }

    PxVec3 origin(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    PxVec3 dir(RayDirection.x, RayDirection.y, RayDirection.z);

    PxRaycastBuffer hitBuffer;

    auto scene = Graphics::get_current_scene();
    if (!scene)
    {
        std::cerr << "Error: current scene is null." << std::endl;
        return false;
    }

    auto physicsWorld = scene->physic_world;
    if (!physicsWorld || !physicsWorld->mScene)
    {
        std::cerr << "Error: physics world or mScene is null." << std::endl;
        return false;
    }

    bool hitCasting = physicsWorld->mScene->raycast(origin, dir, Length, hitBuffer, hitFlags);

    if (hitCasting)
    {
        for (PxU32 i = 0; i < hitBuffer.getNbAnyHits(); i++)
        {
            const PxRaycastHit *hit = &hitBuffer.getAnyHit(i);
            if (!hit)
            {
                std::cerr << "Error: hit is null." << std::endl;
                continue;
            }

            PxShape *actor = hit->shape;
            if (!actor)
            {
                std::cerr << "Error: actor shape is null." << std::endl;
                continue;
            }

            hitRay->point = glm::vec3(hit->position.x, hit->position.y, hit->position.z);
            hitRay->normal = glm::vec3(hit->normal.x, hit->normal.y, hit->normal.z);

            const char *actorName = actor->getName();

            if (!actorName)
            {
                std::cerr << "Error: actor name is null." << std::endl;
                continue;
            }

            std::string nombreDelActor(actorName);
            std::cout << "Object name: " << actorName << std::endl;

            if (std::all_of(nombreDelActor.begin(), nombreDelActor.end(), ::isdigit))
            {
                int objectID = std::stoi(nombreDelActor);
                hitRay->entity = scene->get_entity_by_id(objectID);
                if (!hitRay->entity)
                {
                    std::cerr << "Error: entity not found for ID: " << objectID << std::endl;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                std::cerr << "Error: El nombre del actor no es un número válido: " << nombreDelActor << std::endl;
            }
        }
    }

    return false;
}
