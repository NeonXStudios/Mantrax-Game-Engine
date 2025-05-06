#include "../includes/PhysicsEngine.h"
#include <GarinGraphics.h>
#include <ServiceLocator.h>

void PhysicsEngine::start_world_physics()
{
#pragma region CREATE PHYSICS 3D PHYSX 4.1.2
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
    if (!mFoundation)
    {
        std::cerr << "Error: PxCreateFoundation failed!" << std::endl;
        return;
    }

    std::cout << "Foundation created successfully." << std::endl;

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

    mToleranceScale.length = 100;
    mToleranceScale.speed = 981;
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
    if (!mPhysics)
    {
        std::cerr << "Error: PxCreatePhysics failed!" << std::endl;
        return;
    }
    std::cout << "Physics engine created successfully." << std::endl;

    mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    if (!mDispatcher)
    {
        std::cerr << "Error: PxDefaultCpuDispatcherCreate failed!" << std::endl;
        return;
    }
    std::cout << "CPU dispatcher created successfully." << std::endl;

    physx::PxSceneDesc sceneDesc(mToleranceScale);
    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = PhysicsEngine::CustomFilterShader;

    collision_events = new GarinCollisionEvents();

    sceneDesc.simulationEventCallback = collision_events;

    sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;

    sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP;
    sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKEEP;

    if (!sceneDesc.isValid())
    {
        std::cout << "\nScene Descriptor Validation:" << std::endl;
        std::cout << "CPU Dispatcher: " << (sceneDesc.cpuDispatcher ? "OK" : "Missing") << std::endl;
        std::cout << "Filter Shader: " << (sceneDesc.filterShader ? "OK" : "Missing") << std::endl;
        std::cout << "Gravity: " << sceneDesc.gravity.x << ", " << sceneDesc.gravity.y << ", " << sceneDesc.gravity.z << std::endl;
        std::cerr << "Invalid scene descriptor!" << std::endl;
        return;
    }

    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
    {
        std::cerr << "Error: createScene failed!" << std::endl;
        return;
    }
    std::cout << "Physics scene created successfully." << std::endl;

    physx::PxPvdSceneClient *pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        std::cout << "PVD flags set for scene successfully." << std::endl;
    }

    mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    if (!mMaterial)
    {
        std::cerr << "Error: createMaterial failed!" << std::endl;
        return;
    }
    std::cout << "Material created successfully." << std::endl;

    if (!mScene)
    {
        std::cerr << "Error: mScene no está inicializado antes de crear el ControllerManager." << std::endl;
        return;
    }

    gManager = PxCreateControllerManager(*mScene);
    if (!gManager)
    {
        std::cerr << "Error: PxCreateControllerManager falló al crear el ControllerManager!" << std::endl;
        return;
    }

    std::cout << "Controller manager created successfully." << std::endl;

#pragma endregion
}

void PhysicsEngine::update_world_physics()
{
    SceneManager *sceneM = ServiceLocator::get<SceneManager>().get();

    if (mScene != nullptr && !sceneM->loading_new_scene)
    {
        mScene->simulate(1.0f / Gfx::targetFPS);
        mScene->fetchResults(true);
    }
}

void PhysicsEngine::delete_phys_world()
{
}

void PhysicsEngine::clear_components_in_world()
{
    std::cout << "/**************************************************************************************************************************" << std::endl;
}
