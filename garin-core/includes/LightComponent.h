#pragma once
#include <GarinMaths.h>
#include <GarinComponents.h>

struct PointLight
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float cutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

class LightComponent
{
public:
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
};