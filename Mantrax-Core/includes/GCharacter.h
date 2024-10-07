#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>
#include "Core.h"

class GARINLIBS_API GCharacter : public Component
{
public:
    PxController *gController = nullptr;

    void defines() override;
    void init() override;
    void update() override;
    void move(glm::vec3 move_direction);
    void modifyCapsule(float newHeight, float newRadius);
};