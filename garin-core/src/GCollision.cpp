#include "../includes/GCollision.h"
#include <GarinGraphics.h>

void GCollision::init()
{
    if (Graphics::get_current_scene()->physic_world != nullptr && Graphics::get_current_scene()->physic_world->mScene != nullptr)
    {
        shape = Graphics::get_current_scene()->physic_world->mPhysics->createShape(physx::PxBoxGeometry(boxSize.x, boxSize.y, boxSize.z), *Graphics::get_current_scene()->physic_world->mMaterial, 1);

        std::string nameBODY = std::to_string(entity->objectID);
        shape->setName(entity->ObjectSTRID.c_str());
        std::cout << "Setting shape name to: " << nameBODY << std::endl;
    }
    else
    {
        std::cout << "World physic is null" << std::endl;
    }
}

void GCollision::update()
{
    if (shape)
    {
        physx::PxGeometry *newGeometry = nullptr;

        glm::vec3 newsize = boxSize * entity->get_transform()->Scale;

        newGeometry = new physx::PxBoxGeometry(newsize.x, newsize.y, newsize.z);

        shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, is_trigger);

        shape->setGeometry(*newGeometry);

        std::cout << "Shape Name: " << shape->getName() << std::endl;
    }
}