#include "PhysicsEngine.h"
#include <GarinGraphics.h>

void PhysicsEngine::start_world_physics()
{

    RegisterDefaultAllocator();
    Trace = TraceImpl;
    JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

    Factory::sInstance = new Factory();
    RegisterTypes();

    mTempAllocator = new TempAllocatorImpl(10 * 1024 * 1024);
    mJobSystem = new JobSystemThreadPool(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

    mPhysicsSystem = new PhysicsSystem();

    mPhysicsSystem->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints,
                         mBroadPhaseLayerInterface, mObjectVsBroadPhaseLayerFilter,
                         mObjectVsObjectLayerFilter);

    MyBodyActivationListener body_activation_listener;
    mPhysicsSystem->SetBodyActivationListener(&body_activation_listener);

    MyContactListener contact_listener;
    mPhysicsSystem->SetContactListener(&contact_listener);

    mBodyInterface = &mPhysicsSystem->GetBodyInterface();

    if (mBodyInterface != nullptr && mPhysicsSystem != nullptr && mJobSystem != nullptr && mTempAllocator != nullptr)
    {
        std::cout << "Todos los sistemas de JOLT PHYSICS fueron cargados correctamente" << std::endl;
    }
    else
    {
        std::cout << "Hubo un error durante el inicio de JOLT PHYSICS POR FAVOR VERIFICA LOS SISTEMAS" << std::endl;
    }

    BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
    floor_shape_settings.SetEmbedded();

    ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
    ShapeRefC floor_shape = floor_shape_result.Get();

    BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, 25.0_r, 0.0_r),
                                        Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);

    floor = mBodyInterface->CreateBody(floor_settings);
    // floor->SetLinearVelocity(Vec3(0.0f, -0.2f, 0.0f));
    mBodyInterface->AddBody(floor->GetID(), EActivation::Activate);
    mBodyInterface->SetAngularVelocity(floor->GetID(), Vec3(0.0f, -0.2f, 0.0f));

    BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r),
                                         Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
    mSphereID = mBodyInterface->CreateAndAddBody(sphere_settings, EActivation::Activate);
    mBodyInterface->SetLinearVelocity(mSphereID, Vec3(0.0f, -5.0f, 0.0f));
}

void PhysicsEngine::update_world_physics()
{
    mPhysicsSystem->OptimizeBroadPhase();
    mPhysicsSystem->Update(1.0f / 120.0f, 1, mTempAllocator, mJobSystem);
    Graphics::get_current_scene()->get_entity_by_index(7)->get_transform()->Position.y = mBodyInterface->GetCenterOfMassPosition(floor->GetID()).GetY();
}