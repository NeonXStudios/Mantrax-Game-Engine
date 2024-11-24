#include "../includes/GCharacter.h"
#include <GarinGraphics.h>

void GCharacter::defines()
{
    GVAR(Height, 2.0f, float);
    GVAR(Radius, 2.0f, float);
}

void GCharacter::init()
{
    if (SceneManager::GetOpenScene()->physic_world->mScene == nullptr)
    {
        std::cout << "physxScene Component Null" << std::endl;
    }

    if (SceneManager::GetOpenScene()->physic_world->mMaterial == nullptr)
    {
        std::cout << "physxMaterial Component Null" << std::endl;
    }

    auto *physxScene = SceneManager::GetOpenScene()->physic_world->mScene;
    auto *physxMaterial = SceneManager::GetOpenScene()->physic_world->mMaterial;

    PxCapsuleControllerDesc desc;
    desc.height = GETVAR(Height, float);
    desc.radius = GETVAR(Radius, float);
    desc.position = PxExtendedVec3(0, 5, 0);
    desc.material = physxMaterial;

    gController = SceneManager::GetOpenScene()->physic_world->gManager->createController(desc);

    if (!gController)
    {
        std::cout << "Failed to create the character controller!" << std::endl;
        return;
    }

    gController->setPosition(physx::PxExtendedVec3(entity->get_transform()->Position.x, entity->get_transform()->Position.y, entity->get_transform()->Position.z));
}

void GCharacter::update()
{
    if (entity != nullptr && gController)
    {
        entity->get_transform()->Position = glm::vec3(gController->getPosition().x, gController->getPosition().y, gController->getPosition().z);
    }
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

bool GCharacter::is_valid_float(float value)
{
    return std::isfinite(value);
}

void GCharacter::move(glm::vec3 move_direction)
{
    if (!is_valid_float(move_direction.x) || !is_valid_float(move_direction.y) || !is_valid_float(move_direction.z))
    {
        std::cerr << "Invalid move_direction: NaN detected" << std::endl;
        return;
    }

    try
    {
        if (gController)
        {
            gController->move(physx::PxVec3((float)move_direction.x, (float)move_direction.y, (float)move_direction.z), 0.01f, 1.0f, PxControllerFilters());
        }
        else
        {
            std::cout << "gController is null, cannot move!" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
