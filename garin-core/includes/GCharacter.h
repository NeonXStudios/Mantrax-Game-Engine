#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>

class GCharacter : public Component
{
public:
    PxController *gController = nullptr;

    void init() override;
    void update() override;
    void move(glm::vec3 move_direction);
};