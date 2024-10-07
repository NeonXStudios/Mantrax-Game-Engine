#include "../includes/GCharacter.h"
#include <GarinGraphics.h>

void GCharacter::defines()
{
    GVAR(Height, 2.0f, float);
    GVAR(Radius, 2.0f, float);
}

void GCharacter::init()
{
    auto *physxScene = SceneManager::GetOpenScene()->physic_world->mScene;
    auto *physxMaterial = SceneManager::GetOpenScene()->physic_world->mMaterial;

    PxCapsuleControllerDesc desc;
    desc.height = GETVAR(Height, float);
    desc.radius = GETVAR(Radius, float);
    desc.position = PxExtendedVec3(0, 5, 0);
    desc.material = physxMaterial;

    gController = SceneManager::GetOpenScene()->physic_world->gManager->createController(desc);

    gController->setPosition(physx::PxExtendedVec3(entity->get_transform()->Position.x, entity->get_transform()->Position.y, entity->get_transform()->Position.z));
}

void GCharacter::update()
{

    entity->get_transform()->Position = glm::vec3(gController->getPosition().x, gController->getPosition().y, gController->getPosition().z);
}

void GCharacter::modifyCapsule(float newHeight, float newRadius)
{
    if (gController)
    {
        gController->release();

        PxCapsuleControllerDesc desc;
        desc.height = newHeight;
        desc.radius = newRadius;
        desc.position = gController->getPosition();
        desc.material = SceneManager::GetOpenScene()->physic_world->mMaterial;

        gController = SceneManager::GetOpenScene()->physic_world->gManager->createController(desc);
    }
}

void GCharacter::move(glm::vec3 move_direction)
{
    gController->move(physx::PxVec3(move_direction.x, move_direction.y, move_direction.z), 0.01f, 1.0f, PxControllerFilters());
}
