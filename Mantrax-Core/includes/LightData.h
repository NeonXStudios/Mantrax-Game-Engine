#pragma once
#include <GarinMaths.h>

struct DirectionalLightData {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    bool enabled;
    bool castShadows;
};

struct PointLightData {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float range;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};

struct SpotLightData {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float range;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};
