#include "../includes/GCharacter.h"
#include <GarinGraphics.h>

void GCharacter::init()
{
    // Asegúrate de tener acceso correcto a la escena de PhysX y al material
    auto *physxScene = Graphics::get_current_scene()->physic_world->mScene;
    auto *physxMaterial = Graphics::get_current_scene()->physic_world->mMaterial;

    // Configura la descripción del controlador de cápsula
    PxCapsuleControllerDesc desc;
    desc.height = 2.0f;
    desc.radius = 0.5f;
    desc.position = PxExtendedVec3(0, 5, 0); // Ajusta la posición inicial según tu escena
    desc.material = physxMaterial;

    // Crea el controlador de personaje
    gController = Graphics::get_current_scene()->physic_world->gManager->createController(desc);

    gController->setPosition(physx::PxExtendedVec3(entity->get_transform()->Position.x, entity->get_transform()->Position.y, entity->get_transform()->Position.z));
}

void GCharacter::update()
{
    entity->get_transform()->Position = glm::vec3(gController->getPosition().x, gController->getPosition().y, gController->getPosition().z);
}

void GCharacter::move(glm::vec3 move_direction)
{
    gController->move(physx::PxVec3(move_direction.x, move_direction.y, move_direction.z), 0.01f, 1.0f, PxControllerFilters());
}
