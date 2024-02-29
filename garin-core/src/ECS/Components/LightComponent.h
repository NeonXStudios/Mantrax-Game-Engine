#pragma once
#include <iostream>
#include <ECS.h>
#include <GarinCore.h>

enum light_type
{
    Directional,
    Point,
    Spot
};

class LightComponent : public Component
{
public:
    light_type type_light = Directional;
    float sun_intensity = 0.3f;
    float sun_diffuse = 5.0f;
    float normal_intensity = 0.6f;
    float light_radius = 5.0f;

    float constant = 0.1;
    float linear = 0.09;
    float quadratic = 0.032;

    float cut_off = 30;
    float other_cut_off = 35;

    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    float offset_y = 0.0f;
    float offset_x = 0.0f;
    float offset_z = 0.0f;

    float h1;
    float h2;
    float h3;
    float h1Sun = 60;
    float h2Sun = -45;
    float h3Sun;

    void init() override;
    void update() override;
};