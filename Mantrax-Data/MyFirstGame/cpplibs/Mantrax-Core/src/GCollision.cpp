#include "../includes/GCollision.h"
#include <GarinGraphics.h>

void GCollision::defines()
{
    GVAR(IsTrigger, false, bool);
}

void GCollision::init()
{
    if (SceneManager::GetOpenScene()->physic_world != nullptr && SceneManager::GetOpenScene()->physic_world->mScene != nullptr)
    {
        shape = SceneManager::GetOpenScene()->physic_world->mPhysics->createShape(physx::PxBoxGeometry(boxSize.x, boxSize.y, boxSize.z), *SceneManager::GetOpenScene()->physic_world->mMaterial, true);

        if (shape)
        {
            shape->setName(entity->ObjectSTRID.c_str());

            PxFilterData filterData;
            filterData.word0 = entity->Layer;

            shape->setSimulationFilterData(filterData);
            shape->setQueryFilterData(filterData);

            if (GETVAR(IsTrigger, bool))
            {
                shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
            }
        }
        else
        {
            std::cout << "Error creating shape" << std::endl;
        }
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
        glm::vec3 newsize = boxSize * entity->get_transform()->Scale;

        physx::PxBoxGeometry newGeometry(newsize.x, newsize.y, newsize.z);
        shape->setGeometry(newGeometry);

        PxFilterData filterData;
        filterData.word0 = entity->Layer;

        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);

        if (GETVAR(IsTrigger, bool))
        {
            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }
    }
}