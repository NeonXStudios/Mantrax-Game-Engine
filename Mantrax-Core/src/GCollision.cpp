#include "../includes/GCollision.h"
#include <GarinGraphics.h>

void GCollision::defines()
{
    GVAR(IsTrigger, false, bool);
    GVAR(boxSize, glm::vec3(1.0f), glm::vec3);
}

void GCollision::init()
{
    if (!SceneManager::get_scene_manager()->physic_world->mPhysics || !SceneManager::get_scene_manager()->physic_world || !SceneManager::get_scene_manager()->physic_world->mScene)
    {
        std::cerr << "Error: Recursos de PhysX no están inicializados." << std::endl;
        return;
    }

    if (SceneManager::get_scene_manager()->physic_world != nullptr && SceneManager::get_scene_manager()->physic_world->mScene != nullptr)
    {
        shape = SceneManager::get_scene_manager()->physic_world->mPhysics->createShape(physx::PxBoxGeometry(boxSize.x, boxSize.y, boxSize.z), *SceneManager::get_scene_manager()->physic_world->mMaterial, true);

        if (shape)
        {
            shape->setName(entity->object_string_id.c_str());
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
        glm::vec3 newsize = GETVAR(boxSize, glm::vec3) * entity->get_transform()->Scale;
        physx::PxBoxGeometry newGeometry(newsize.x, newsize.y, newsize.z);
        shape->setGeometry(newGeometry);

        PxFilterData filterData;
        if (GETVAR(IsTrigger, bool))
        {
            filterData.word0 = LAYER_TRIGGER;
            filterData.word1 = LAYER_PLAYER | LAYER_ENEMY;

            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
        }
        else
        {
            filterData.word0 = entity->Layer;
            filterData.word1 = entity->Layer | LAYER_ENEMY | LAYER_ENVIRONMENT;

            shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
            shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
        }

        shape->setSimulationFilterData(filterData);
        shape->setQueryFilterData(filterData);
    }
}

void GCollision::clean()
{
}