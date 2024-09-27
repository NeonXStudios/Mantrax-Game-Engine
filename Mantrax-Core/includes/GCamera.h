#pragma once
#include <ECS.h>
#include <camera.h>
#include <Core.h>

class GARINLIBS_API GCamera : public Component
{
public:
    Camera *a_camera;

    void defines() override;
    void init() override;
    void update() override;
    void clean() override;
};