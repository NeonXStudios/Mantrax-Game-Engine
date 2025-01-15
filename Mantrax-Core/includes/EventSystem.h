#pragma once
#include "GarinGraphics.h"
#include "Core.h"
#include <GarinMaths.h>
#include <RenderPipeline.h>
#include <UIBehaviour.h>
#include <SceneManager.h>

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

    static glm::vec2 screen_to_viewport();
    static vec2 get_mouse_position_in_viewport(glm::vec2 WindowSize, glm::vec2 ScreenSize);
    static glm::vec2 mouse_to_view_port_position(glm::vec2 WindowSize);
    static glm::vec2 mouse_to_screen_pos(glm::vec2 WindowSize);
    static bool MouseCast2D(glm::vec2 coords, CastData *data);
    static bool MouseCastUI(glm::vec2 coords, CastDataUI *data);
    static bool MouseCast3D(const glm::vec2& screenCoords, CastData* data);

    // Función que verifica si un rayo intersecta un AABB
    static bool RayIntersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& min, const glm::vec3& max, float* tmin_out)
    {
        float tmin = (min.x - rayOrigin.x) / rayDirection.x;
        float tmax = (max.x - rayOrigin.x) / rayDirection.x;

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (min.y - rayOrigin.y) / rayDirection.y;
        float tymax = (max.y - rayOrigin.y) / rayDirection.y;

        if (tymin > tymax) std::swap(tymin, tymax);

        if (tmin > tymax || tymin > tmax)
            return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min.z - rayOrigin.z) / rayDirection.z;
        float tzmax = (max.z - rayOrigin.z) / rayDirection.z;

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        if (tmin > tzmax || tzmin > tmax)
            return false;

        if (tmin_out) *tmin_out = tmin;  // Pasamos tmin si es necesario

        return true;
    }


    // static bool MouseCastNonScene(glm::vec2 coords, CastData *data, std::vector<Entity *> entitys)
    // {
    //     float closestZ = std::numeric_limits<float>::lowest();
    //     Entity *closestObject = nullptr;

    //     for (int i = 0; i < entitys.size(); i++)
    //     {
    //         Entity *objD = entitys[i];

    //         glm::vec3 &obj = objD->get_transform()->Position;
    //         float objWidth = objD->get_transform()->Scale.x;
    //         float objHeight = objD->get_transform()->Scale.y;

    //         float radians = objD->get_transform()->rotation.x;

    //         glm::vec2 localPoint = RotatePoint(coords, obj, radians);

    //         // Solo considera la coordenada Z para determinar qué objeto está más adelante
    //         if (localPoint.x >= obj.x - objWidth && localPoint.x <= obj.x + objWidth &&
    //             localPoint.y >= obj.y - objHeight && localPoint.y <= obj.y + objHeight)
    //         {

    //             // Comprueba si la coordenada Z es mayor que la más cercana hasta ahora
    //             if (obj.z > closestZ)
    //             {
    //                 closestZ = obj.z;
    //                 closestObject = objD;
    //             }
    //         }
    //     }

    //     if (closestObject != nullptr)
    //     {
    //         data->object = closestObject;
    //         return true;
    //     }

    //     return false;
    // }

    static glm::vec2 RotatePoint(const glm::vec2 &point, const glm::vec2 &center, float angle);
};