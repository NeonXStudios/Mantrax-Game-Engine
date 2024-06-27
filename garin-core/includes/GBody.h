#pragma once
#include <ECS.h>
#include <GarinMaths.h>
#include <PhysicsEngine.h>

class GBody : public Component
{
public:
    void init() override;
    void update() override;

    glm::vec3 get_body_position();
    glm::quat get_body_rotation();
};