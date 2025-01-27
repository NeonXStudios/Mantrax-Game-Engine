#pragma once
#include "GarinGraphics.h"
#include "Core.h"
#include <RenderPipeline.h>
#include <UIBehaviour.h>
#include <SceneManager.h>
#include <algorithm> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility> 


struct GARINLIBS_API CastData {
    Entity* object;
    glm::vec3 hitPoint;
    float distance;
};

struct GARINLIBS_API CastDataUI
{
    UIBehaviour *object;
};

class GARINLIBS_API EventSystem
{
public:
    static glm::vec2 ViewportRenderPosition;

    static glm::vec2 screen_to_viewport(Camera* cam);
    static vec2 get_mouse_position_in_viewport(glm::vec2 WindowSize, glm::vec2 ScreenSize);
    static glm::vec2 mouse_to_view_port_position(glm::vec2 WindowSize);
    static glm::vec2 mouse_to_screen_pos(glm::vec2 WindowSize);
    static bool MouseCast2D(glm::vec2 coords, CastData *data, Camera* camera);
    static bool MouseCastUI(glm::vec2 coords, CastDataUI *data);
    static bool MouseCast3D(const glm::vec2& screenCoords, CastData* data);

    static bool RayIntersectsAABB(
        const glm::vec3 &rayOrigin,
        const glm::vec3 &rayDirection,
        const glm::vec3 &boxMin,
        const glm::vec3 &boxMax)
    {
        float tMin = (boxMin.x - rayOrigin.x) / rayDirection.x;
        float tMax = (boxMax.x - rayOrigin.x) / rayDirection.x;

        if (tMin > tMax) std::swap(tMin, tMax);

        float tyMin = (boxMin.y - rayOrigin.y) / rayDirection.y;
        float tyMax = (boxMax.y - rayOrigin.y) / rayDirection.y;

        if (tyMin > tyMax) std::swap(tyMin, tyMax);

        if ((tMin > tyMax) || (tyMin > tMax))
            return false;

        if (tyMin > tMin)
            tMin = tyMin;

        if (tyMax < tMax)
            tMax = tyMax;

        float tzMin = (boxMin.z - rayOrigin.z) / rayDirection.z;
        float tzMax = (boxMax.z - rayOrigin.z) / rayDirection.z;

        if (tzMin > tzMax) std::swap(tzMin, tzMax);

        if ((tMin > tzMax) || (tzMin > tMax))
            return false;

        return true;
    }

    static void ScreenToWorldRay(
        glm::vec2 mouseCoords,
        const glm::mat4& viewMatrixInverse,
        const glm::mat4& projectionMatrixInverse,
        glm::vec3& rayOrigin,
        glm::vec3& rayDirection,
        Camera* cam)
    {
        glm::vec2 normalizedMouseCoords = EventSystem::screen_to_viewport(cam);
        
        glm::vec4 clipNear = glm::vec4(normalizedMouseCoords.x, normalizedMouseCoords.y, -1.0f, 1.0f);
        glm::vec4 clipFar = glm::vec4(normalizedMouseCoords.x, normalizedMouseCoords.y, 1.0f, 1.0f);
        
        glm::vec4 viewNear = projectionMatrixInverse * clipNear;
        glm::vec4 viewFar = projectionMatrixInverse * clipFar;
        
        viewNear /= viewNear.w;
        viewFar /= viewFar.w;
        
        glm::vec4 worldNear = viewMatrixInverse * viewNear;
        glm::vec4 worldFar = viewMatrixInverse * viewFar;
        
        rayOrigin = glm::vec3(worldNear);
        rayDirection = glm::normalize(glm::vec3(worldFar - worldNear));
    }

    static bool RayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
                            const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t)
    {
        const float EPSILON = 0.0001f;

        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;

        glm::vec3 h = glm::cross(rayDir, e2);
        float a = glm::dot(e1, h);

        if (a > -EPSILON && a < EPSILON)
            return false;

        float f = 1.0f / a;
        glm::vec3 s = rayOrigin - v0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f)
            return false;

        glm::vec3 q = glm::cross(s, e1);
        float v = f * glm::dot(rayDir, q);

        if (v < 0.0f || u + v > 1.0f)
            return false;

        t = f * glm::dot(e2, q);

        return t > EPSILON;
    }

    static float min3(float a, float b, float c) {
        float temp = (a < b) ? a : b;
        return (temp < c) ? temp : c;
    }

    static float max3(float a, float b, float c) {
        float temp = (a > b) ? a : b;
        return (temp > c) ? temp : c;
    }


    static glm::vec2 RotatePoint(const glm::vec2 &point, const glm::vec2 &center, float angle);
};