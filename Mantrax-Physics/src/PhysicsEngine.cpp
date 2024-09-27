#include "../includes/PhysicsEngine.h"
#include <GarinGraphics.h>

void PhysicsEngine::start_world_physics()
{
#pragma region CREATE PHYSICS 3D PHYSX 4.1.2
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
    if (!mFoundation)
        throw("PxCreateFoundation failed!");
    mPvd = PxCreatePvd(*mFoundation);
    physx::PxPvdTransport *transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    // mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(),true, mPvd);
    mToleranceScale.length = 100; // typical length of an object
    mToleranceScale.speed = 981;  // typical speed of an object, gravity*1s is a reasonable choice
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
    // mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);

    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);

    physx::PxPvdSceneClient *pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    // create simulation
    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    GarinCollisionEvents *collisionCallback = new GarinCollisionEvents();

    mScene->setSimulationEventCallback(collisionCallback);

    if (mFoundation)
        std::cout << "Physic world started with PhysX" << std::endl;

    // Crea el manager del controlador de personaje
    gManager = PxCreateControllerManager(*mScene);
#pragma endregion
}

void PhysicsEngine::update_world_physics()
{
    mScene->simulate(1.0f / 60.0f);
    mScene->fetchResults(true);
}

void PhysicsEngine::delete_phys_world()
{
    try
    {

        if (gManager)
        {
            gManager->purgeControllers();
            gManager->release();
        }

        if (mScene)
        {
            PxU32 numActors = mScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
            std::vector<PxRigidActor *> actors(numActors);
            mScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor **>(actors.data()), numActors);

            for (PxRigidActor *actor : actors)
            {
                PxU32 numShapes = actor->getNbShapes();
                std::vector<PxShape *> shapes(numShapes);
                actor->getShapes(shapes.data(), numShapes);

                for (PxShape *shape : shapes)
                {
                    shape->release();
                }

                actor->release();
            }

            mScene->release();
            mScene = nullptr;
        }

        if (mMaterial)
        {
            mMaterial->release();
            mMaterial = nullptr;
        }

        if (mDispatcher)
        {
            mDispatcher->release();
            mDispatcher = nullptr;
        }

        if (mPhysics)
        {
            mPhysics->release();
            mPhysics = nullptr;
        }

        if (mPvd)
        {
            mPvd->release();
            mPvd = nullptr;
        }

        if (mFoundation)
        {
            mFoundation->release();
            mFoundation = nullptr;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}