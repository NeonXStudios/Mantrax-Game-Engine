#include "../includes/GBody.h"
#include "../includes/GCollision.h"
#include <GarinGraphics.h>
#include <SceneManager.h>

void GBody::defines()
{
    GVAR(mass, 1.0f, float);
    GVAR(isStatic, false, bool);
    GVAR(useGravity, true, bool);
}

void GBody::init()
{
    try
    {
        if (!SceneManager::get_scene_manager()->physic_world->mPhysics || !SceneManager::get_scene_manager()->physic_world || !SceneManager::get_scene_manager()->physic_world->mScene)
        {
            std::cerr << "Error: Recursos de PhysX no están inicializados." << std::endl;
            return;
        }
        mPhysics = SceneManager::get_scene_manager()->physic_world->mPhysics;

        physx::PxVec3 position_start = physx::PxVec3(
            entity->get_transform()->getPosition().x,
            entity->get_transform()->getPosition().y,
            entity->get_transform()->getPosition().z);

        physx::PxQuat quat_start = physx::PxQuat(
            entity->get_transform()->getRotation().x,
            entity->get_transform()->getRotation().y,
            entity->get_transform()->getRotation().z,
            entity->get_transform()->getRotation().w);

        physx::PxTransform t(position_start, quat_start);

        body = mPhysics->createRigidDynamic(t);
        body->setName(entity->object_string_id.c_str());

        physx::PxRigidBodyExt::updateMassAndInertia(*body, GETVAR(mass, float));
        SceneManager::get_scene_manager()->physic_world->mScene->addActor(*body);

        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, GETVAR(isStatic, bool));
        body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !GETVAR(useGravity, bool));

        if (entity->hasComponent<GCollision>())
        {
            auto &collisions = entity->getComponents<GCollision>();
            for (auto &collision : collisions)
            {
                if (collision->shape && std::find(attachedShapes.begin(), attachedShapes.end(), collision->shape) == attachedShapes.end())
                {
                    body->attachShape(*collision->shape);
                    attachedShapes.push_back(collision->shape);
                }
            }
        }

        body->wakeUp();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void GBody::update()
{
    if (body != nullptr && SceneManager::get_scene_manager()->physic_world != nullptr && SceneManager::get_scene_manager()->physic_world->mScene != nullptr)
    {
        if (entity->hasComponent<GCollision>())
        {
            auto &collisions = entity->getComponents<GCollision>();
            for (auto &collision : collisions)
            {
                if (collision->shape && std::find(attachedShapes.begin(), attachedShapes.end(), collision->shape) == attachedShapes.end())
                {
                    body->attachShape(*collision->shape);
                    attachedShapes.push_back(collision->shape);
                }
            }
        }

        physx::PxQuat rotation = body->getGlobalPose().q;
        entity->get_transform()->rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);

        body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, GETVAR(isStatic, bool));
        body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !GETVAR(useGravity, bool));

        entity->get_transform()->setPosition(get_body_position());

        if (GETVAR(useGravity, bool))
        {
            body->wakeUp();
        }
    }
}

glm::vec3 GBody::get_body_position()
{
    float x = body->getGlobalPose().p.x;
    float y = body->getGlobalPose().p.y;
    float z = body->getGlobalPose().p.z;

    return glm::vec3(x, y, z);
}

void GBody::add_impulse(glm::vec3 direction, GBodySpace::force_mode mode)
{
    switch (mode)
    {
    case GBodySpace::Impulse:
    {
        body->addForce(physx::PxVec3(direction.x, direction.y, direction.z), physx::PxForceMode::eIMPULSE, true);
        break;
    }
    case GBodySpace::Acceleration:
    {
        body->addForce(physx::PxVec3(direction.x, direction.y, direction.z), physx::PxForceMode::eACCELERATION, true);
        break;
    }
    case GBodySpace::Velocity:
    {
        body->addForce(physx::PxVec3(direction.x, direction.y, direction.z), physx::PxForceMode::eVELOCITY_CHANGE, true);
        break;
    }
    }
}

void GBody::set_position(glm::vec3 new_position)
{
    physx::PxVec3 px_position(new_position.x, new_position.y, new_position.z);

    physx::PxTransform current_pose = body->getGlobalPose();

    current_pose.p = px_position;

    body->setGlobalPose(current_pose);
}

void GBody::clean()
{
}
