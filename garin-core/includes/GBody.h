#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>

namespace GBodySpace
{
    enum force_mode
    {
        Impulse,
        Velocity,
        Acceleration
    };
};

class GBody : public Component
{
public:
    // float mass = 1;
    float drag = 0;
    float angularDrag = 0.05f;
    bool automaticCenterOfMass = true;
    bool automaticSensor = true;

    float friction = 0.5f;

    // bool isStatic = false;
    // bool useGravity = true;

    bool freezePosX = false;
    bool freezePosY = false;
    bool freezePosZ = false;

    bool freezeRotX = false;
    bool freezeRotY = false;
    bool freezeRotZ = false;

    bool shapeAttached;

    physx::PxRigidDynamic *body;
    physx::PxMaterial *mMaterial = NULL;
    physx::PxPhysics *mPhysics = NULL;

    bool waked;

    void defines() override;
    void init() override;
    void update() override;

    void add_impulse(glm::vec3 direction, GBodySpace::force_mode mode);

    glm::vec3 get_body_position();
    glm::quat get_body_rotation();
};