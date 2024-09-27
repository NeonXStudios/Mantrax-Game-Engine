#pragma once
#include <GarinComponents.h>

class GizmoModels
{
public:
    // COMPONENTS
    Model *camera_gizmo = nullptr;
    Shader* p_shader = nullptr;

    // FUNCTIONS
    void init();
    void draw();
};