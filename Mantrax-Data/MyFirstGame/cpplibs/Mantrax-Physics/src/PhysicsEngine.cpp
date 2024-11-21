#include "../includes/PhysicsEngine.h"
#include <GarinGraphics.h>

void PhysicsEngine::start_world_physics()
{
#pragma region CREATE PHYSICS 3D PHYSX 4.1.2

    // Crear Foundation
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
    if (!mFoundation)
    {
        std::cerr << "Error: PxCreateFoundation failed!" << std::endl;
        return;
    }
    std::cout << "Foundation created successfully." << std::endl;

    // Crear PVD (PhysX Visual Debugger) y conectar
    mPvd = PxCreatePvd(*mFoundation);
    if (!mPvd)
    {
        std::cerr << "Error: PxCreatePvd failed!" << std::endl;
        return;
    }
    physx::PxPvdTransport *transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    if (!transport)
    {
        std::cerr << "Error: PxDefaultPvdSocketTransportCreate failed!" << std::endl;
        return;
    }
    mPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    std::cout << "PVD created and connected successfully." << std::endl;

    // Crear el motor de física
    mToleranceScale.length = 100; // typical length of an object
    mToleranceScale.speed = 981;  // typical speed of an object, gravity*1s is a reasonable choice
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
    if (!mPhysics)
    {
        std::cerr << "Error: PxCreatePhysics failed!" << std::endl;
        return;
    }
    std::cout << "Physics engine created successfully." << std::endl;

    // Configurar la escena de física
    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    // Crear dispatcher (hilo de simulación)
    mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    if (!mDispatcher)
    {
        std::cerr << "Error: PxDefaultCpuDispatcherCreate failed!" << std::endl;
        return;
    }
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    std::cout << "CPU dispatcher created successfully." << std::endl;

    // Crear la escena de física
    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
    {
        std::cerr << "Error: createScene failed!" << std::endl;
        return;
    }
    std::cout << "Physics scene created successfully." << std::endl;

    // Configurar PVD en la escena
    physx::PxPvdSceneClient *pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        std::cout << "PVD flags set for scene successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error: PVD client not available!" << std::endl;
    }

    // Crear materiales
    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    if (!mMaterial)
    {
        std::cerr << "Error: createMaterial failed!" << std::endl;
        return;
    }
    std::cout << "Material created successfully." << std::endl;

    // Crear y asignar los eventos de colisión
    GarinCollisionEvents *collisionCallback = new GarinCollisionEvents();
    if (collisionCallback)
    {
        mScene->setSimulationEventCallback(collisionCallback);
        std::cout << "Collision event callback set successfully." << std::endl;
    }
    else
    {
        std::cerr << "Error: Collision event callback creation failed!" << std::endl;
        return;
    }

    // Crear el controlador para manejar personajes o entidades
    if (!mFoundation)
    {
        std::cerr << "Error: Foundation is null while creating controller manager!" << std::endl;
        return;
    }
    gManager = PxCreateControllerManager(*mScene);
    if (!gManager)
    {
        std::cerr << "Error: PxCreateControllerManager failed!" << std::endl;
        return;
    }
    std::cout << "Controller manager created successfully." << std::endl;

#pragma endregion
}



void PhysicsEngine::update_world_physics()
{
    if (mScene != nullptr){
        mScene->simulate(1.0f / 60.0f);
        mScene->fetchResults(true);
    }
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