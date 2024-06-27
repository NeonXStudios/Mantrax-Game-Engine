#include "../includes/GBody.h"
#include <GarinGraphics.h>

void GBody::init()
{
    mPhysics = Graphics::get_current_scene()->physic_world->mPhysics;

    // shape = mPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *SceneManager::GetSceneManager()->OpenScene->mMaterial, 1);
    physx::PxTransform t(physx::PxVec3(0));

    // Crear un objeto dinámico
    body = mPhysics->createRigidDynamic(t);

    string nameBODY = std::to_string(entity->objectID);
    body->setName(nameBODY.c_str());

    // body->attachShape(*shape);
    physx::PxRigidBodyExt::updateMassAndInertia(*body, mass);
    Graphics::get_current_scene()->physic_world->mScene->addActor(*body);

    body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isStatic);
    body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !useGravity);

    body->getGlobalPose().p = physx::PxVec3(entity->get_transform()->Position.x, entity->get_transform()->Position.y, entity->get_transform()->Position.z);

    body->wakeUp();
}

void GBody::update()
{
    entity->get_transform()->Position = get_body_position();
}

glm::vec3 GBody::get_body_position()
{
    float x = body->getGlobalPose().p.x;
    float y = body->getGlobalPose().p.y;
    float z = body->getGlobalPose().p.z;

    return glm::vec3(x, y, z);
}