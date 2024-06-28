#include "../includes/GCollision.h"
#include <GarinGraphics.h>

void GCollision::init()
{
    mPhysics = Graphics::get_current_scene()->physic_world->mPhysics;
    shape = mPhysics->createShape(physx::PxBoxGeometry(boxSize.x, boxSize.y, boxSize.z), *Graphics::get_current_scene()->physic_world->mMaterial, 1);
    shape->setName(entity->ObjectSTRID.c_str());

    string nameBODY = std::to_string(entity->objectID);
    shape->setName(nameBODY.c_str());
}

void GCollision::update()
{
    physx::PxGeometry *newGeometry = nullptr;

    glm::vec3 newsize = boxSize * entity->get_transform()->Scale;

    newGeometry = new physx::PxBoxGeometry(newsize.x, newsize.y, newsize.z);
    shape->setGeometry(*newGeometry);
}
