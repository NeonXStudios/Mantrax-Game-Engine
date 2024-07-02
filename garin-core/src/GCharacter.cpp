#include "../includes/GCharacter.h"
#include <GarinGraphics.h>

void GCharacter::init()
{
    gManager = PxCreateControllerManager(*Graphics::get_current_scene()->physic_world->mScene);

    PxCapsuleControllerDesc desc;
    desc.height = 2.0f;
    desc.radius = 0.5f;
    desc.position = PxExtendedVec3(0, 5, 0);
    desc.material = Graphics::get_current_scene()->physic_world->mMaterial;

    gController = gManager->createController(desc);
}

void GCharacter::update()
{
}

void GCharacter::move(glm::vec3 move_direction)
{
    gController->move(physx::PxVec3(move_direction.x, move_direction.y, move_direction.z), 0.01f, 1.0f, PxControllerFilters());
}