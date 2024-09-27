#pragma once
#include <GarinMaths.h>
#include <GarinComponents.h>
#include "Core.h"

struct GARINLIBS_API PointLight
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

struct GARINLIBS_API SpotLight
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

class GARINLIBS_API LightComponent
{
public:
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
};