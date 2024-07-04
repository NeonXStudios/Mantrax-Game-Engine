#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>

class GCollision : public Component
{
public:
    glm::vec3 boxSize = glm::vec3(1, 1, 1);
    bool is_trigger;

    physx::PxShape *shape = NULL;

    void init() override;
    void update() override;
};
