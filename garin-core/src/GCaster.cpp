#include "../includes/GCaster.h"
#include <GarinGraphics.h>

bool GCaster::RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Lenght, GCastHit *hitRay, PxHitFlags hitFlags)
{

    PxVec3 origin = PxVec3(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    PxVec3 dir = PxVec3(RayDirection.x, RayDirection.y, RayDirection.z);

    PxRaycastBuffer hitBuffer;

    bool hitCasting = Graphics::get_current_scene()->physic_world->mScene->raycast(origin, dir, Lenght, hitBuffer);

    if (hitCasting)
    {
        for (PxU32 i = 0; i < hitBuffer.getNbAnyHits(); i++)
        {
            const PxRaycastHit *hit = &hitBuffer.getAnyHit(i);
            PxShape *actor = hit->shape;
            hitRay->point = glm::vec3(hit->position.x, hit->position.y, hit->position.z);
            hitRay->normal = glm::vec3(hit->normal.x, hit->normal.y, hit->normal.z);

            std::string nombreDelActor = actor->getName();

            hitRay->entity = Graphics::get_current_scene()->get_entity_by_id(std::stoi(nombreDelActor));
            return true;
        }
    }

    return false;
}