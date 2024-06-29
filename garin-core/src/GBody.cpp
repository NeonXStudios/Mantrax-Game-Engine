#include "../includes/GBody.h"
#include "../includes/GCollision.h"
#include <GarinGraphics.h>

void GBody::defines()
{
    GVAR(mass, 1.0f);
}

void GBody::init()
{

    mPhysics = Graphics::get_current_scene()->physic_world->mPhysics;

    // shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *SceneManager::GetSceneManager()->OpenScene->mMaterial, 1);
    physx::PxVec3 position_start = physx::PxVec3(entity->get_transform()->Position.x, entity->get_transform()->Position.y, entity->get_transform()->Position.z);
    glm::quat quat_conversion = glm::quat(glm::radians(glm::vec3(
        entity->get_transform()->rotation.x,
        entity->get_transform()->rotation.y,
        entity->get_transform()->rotation.z)));

    // Crear el PxQuat para PhysX
    physx::PxTransform t(position_start, physx::PxQuat(
                                             entity->get_transform()->rotation.x, entity->get_transform()->rotation.y, entity->get_transform()->rotation.z, entity->get_transform()->rotation.w));

    body = mPhysics->createRigidDynamic(t);

    string nameBODY = std::to_string(entity->objectID);
    body->setName(nameBODY.c_str());

    physx::PxRigidBodyExt::updateMassAndInertia(*body, GETVAR(mass, float));
    Graphics::get_current_scene()->physic_world->mScene->addActor(*body);

    body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isStatic);
    body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity);

    body->wakeUp();
}

void GBody::update()
{

    if (entity->hasComponent<GCollision>() && !shapeAttached)
    {
        body->attachShape(*entity->getComponent<GCollision>().shape);
        shapeAttached = true;
    }
    else
    {
        if (!entity->hasComponent<GCollision>())
        {
            shapeAttached = false;
        }
    }

    physx::PxQuat rotation = body->getGlobalPose().q;

    double gradosX = rotation.x * (180.0 / M_PI);
    double gradosY = rotation.y * (180.0 / M_PI);
    double gradosZ = rotation.z * (180.0 / M_PI);

    entity->get_transform()->Rotation.x = gradosX;
    entity->get_transform()->Rotation.y = gradosY;
    entity->get_transform()->Rotation.z = gradosZ;

    body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isStatic);
    body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity);

    entity->get_transform()->Position = get_body_position();

    if (useGravity)
    {
        body->wakeUp();
    }
}

void GBody::clean()
{
    PxRigidActor *actor = body;
    Graphics::get_current_scene()->physic_world->mScene->removeActor(*actor);
    actor->release();
}

glm::vec3 GBody::get_body_position()
{
    float x = body->getGlobalPose().p.x;
    float y = body->getGlobalPose().p.y;
    float z = body->getGlobalPose().p.z;

    return glm::vec3(x, y, z);
}