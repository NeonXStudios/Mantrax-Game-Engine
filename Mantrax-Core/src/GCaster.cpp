#include "../includes/GCaster.h"
#include <GarinGraphics.h>
#include <ServiceLocator.h>

bool GCaster::RayCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit *hitRay, PxHitFlags hitFlags, PxU32 raycastLayerMask)
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    PxVec3 rayOrigin = PxVec3(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    PxVec3 rayDir = PxVec3(RayDirection.x, RayDirection.y, RayDirection.z);
    PxReal maxDistance = Length;
    PxRaycastBuffer hit;

    if (sceneM->physic_world->mScene->raycast(rayOrigin, rayDir, maxDistance, hit))
    {
        PxVec3 hitPosition = hit.block.position;
        PxVec3 hitNormal = hit.block.normal;
        PxReal distance = hit.block.distance;

        const char *shapeName = hit.block.shape->getName();
    }
    else
    {
        std::cout << "No hit detected." << std::endl;
    }

    return false;
}

bool GCaster::LineCast(glm::vec3 RayOrigin, glm::vec3 RayDirection, float Length, GCastHit *hitinfo, PxU32 layerMask)
{
    SceneManager* sceneM = ServiceLocator::get<SceneManager>().get();

    PxVec3 rayOrigin = PxVec3(RayOrigin.x, RayOrigin.y, RayOrigin.z);
    PxVec3 rayDir = PxVec3(RayDirection.x, RayDirection.y, RayDirection.z);
    PxReal maxDistance = Length;
    PxRaycastBuffer hit;

    PxQueryFilterData filterData;
    filterData.data.word0 = layerMask;

    if (sceneM->physic_world->mScene->raycast(rayOrigin, rayDir, maxDistance, hit, PxHitFlags(PxHitFlag::eDEFAULT), filterData))
    {
        PxVec3 hitPosition = hit.block.position;
        PxVec3 hitNormal = hit.block.normal;
        PxReal distance = hit.block.distance;

        const char *shapeName = hit.block.shape->getName();

        if (sceneM->get_current_scene()->get_entity_by_id_string(shapeName) != nullptr)
        {
            hitinfo->point = glm::vec3(hitPosition.x, hitPosition.y, hitPosition.z);
            hitinfo->normal = glm::vec3(hitNormal.x, hitNormal.y, hitNormal.z);
            hitinfo->entity = sceneM->get_current_scene()->get_entity_by_id_string(shapeName);
        }
        else
        {
            hitinfo->point = glm::vec3(hitPosition.x, hitPosition.y, hitPosition.z);
            hitinfo->normal = glm::vec3(hitNormal.x, hitNormal.y, hitNormal.z);
            hitinfo->entity = nullptr;
        }

        return true;
    }
    else
    {
        return false;
    }
}
