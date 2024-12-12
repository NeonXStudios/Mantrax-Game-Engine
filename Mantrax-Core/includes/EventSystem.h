#pragma once
#include "GarinGraphics.h"
#include "Core.h"
#include <GarinMaths.h>
#include <RenderPipeline.h>
#include <UIBehaviour.h>

struct GARINLIBS_API CastData
{
    Entity *object;
    glm::vec3 hitPoint;
};

struct GARINLIBS_API CastDataUI
{
    UIBehaviour *object;
};

class GARINLIBS_API EventSystem
{
public:
    static glm::vec2 ScreenToViewPort(glm::vec2 ViewportRenderPosition, glm::vec2 WindowSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        Camera *cam = SceneManager::get_current_scene()->main_camera;

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = (windowMousePosX / Gfx::render_width);
        double NormalMousePosY = (-windowMousePosY / Gfx::render_height);

        // CALCULAR CENTRO DE LA TEXTURA CON EL MOUSE (0, 0)
        double centeredMousePosX = ((NormalMousePosX * 2.0f - 1.0f) * (Gfx::render_width / 2) * cam->zoom);
        double centeredMousePosY = ((NormalMousePosY * 2.0f + 1.0f) * (Gfx::render_height / 2) * cam->zoom);

        double WorldPointX = centeredMousePosX + cam->cameraPosition.x;
        double WorldPointY = centeredMousePosY + cam->cameraPosition.y;

        return glm::vec2(WorldPointX, WorldPointY);
    }

    static vec2 GetMousePositionInViewPort(glm::vec2 ViewportRenderPosition, glm::vec2 WindowSize, glm::vec2 ScreenSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = windowMousePosX / WindowSize.x;
        double NormalMousePosY = -windowMousePosY / WindowSize.y;

        return vec2(0, 0);
    }

    static glm::vec2 MouseToViewPortPosition(glm::vec2 ViewportRenderPosition, glm::vec2 WindowSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);
        Camera *cam = SceneManager::get_open_scene()->main_camera;

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = windowMousePosX;
        double NormalMousePosY = windowMousePosY;

        // CALCULAR CENTRO DE LA TEXTURA CON EL MOUSE (0, 0)
        double centeredMousePosX = (NormalMousePosX * 2.0f - 1.0f) * (Gfx::render_width)*cam->zoom;
        double centeredMousePosY = (NormalMousePosY * 2.0f + 1.0f) * (Gfx::render_height)*cam->zoom;

        double WorldPointX = centeredMousePosX + cam->cameraPosition.x;
        double WorldPointY = centeredMousePosY + cam->cameraPosition.y;

        return glm::vec2(NormalMousePosX, NormalMousePosY);
    }

    static glm::vec2 MouseToScreenPos(glm::vec2 ViewportRenderPosition, glm::vec2 WindowSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = (windowMousePosX / Gfx::render_width);
        double NormalMousePosY = (-windowMousePosY / Gfx::render_height);

        // CALCULAR CENTRO DE LA TEXTURA CON EL MOUSE (0, 0)
        double centeredMousePosX = ((NormalMousePosX * 2.0f - 1.0f) * (Gfx::render_width / 2) * RenderPipeline::canvas->zoom);
        double centeredMousePosY = ((NormalMousePosY * 2.0f + 1.0f) * (Gfx::render_height / 2) * RenderPipeline::canvas->zoom);

        double WorldPointX = centeredMousePosX;
        double WorldPointY = centeredMousePosY;

        return glm::vec2(WorldPointX, WorldPointY);
    }

    // Función para calcular la dirección del rayo en 3D
    static glm::vec3 CalculateRayDirection(const glm::vec2 &mouseCoords, Camera *camera, float screenWidth, float screenHeight)
    {
        // Normalizar las coordenadas del mouse a rango [-1, 1]
        float x = (2.0f * mouseCoords.x) / screenWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseCoords.y) / screenHeight; // Invertir el eje Y si es necesario
        float z = 1.0f;                                         // En NDC, el punto está en el plano z = 1

        glm::vec3 rayNDC = glm::vec3(x, y, z);

        // Convertir a espacio de clip
        glm::vec4 rayClip = glm::vec4(rayNDC, 1.0f);

        // Convertir a espacio de cámara
        glm::mat4 invProjection = glm::inverse(camera->GetProjectionMatrix());
        glm::vec4 rayEye = invProjection * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

        // Convertir a espacio mundial
        glm::mat4 invView = glm::inverse(camera->GetView());
        glm::vec4 rayWorld4 = invView * rayEye;
        glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));

        return rayWorld;
    }

    static bool RayAABBIntersection_Slab(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection,
                                         const glm::vec3 &minAABB, const glm::vec3 &maxAABB,
                                         float &tMin, float &tMax)
    {
        tMin = -std::numeric_limits<float>::infinity();
        tMax = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; ++i)
        {
            if (fabs(rayDirection[i]) < 1e-8)
            {
                // El rayo es paralelo al plano en este eje
                if (rayOrigin[i] < minAABB[i] || rayOrigin[i] > maxAABB[i])
                    return false; // No hay intersección
                else
                    continue; // El rayo está dentro de los planos, no afecta tMin y tMax
            }

            float t1 = (minAABB[i] - rayOrigin[i]) / rayDirection[i];
            float t2 = (maxAABB[i] - rayOrigin[i]) / rayDirection[i];

            if (t1 > t2)
                std::swap(t1, t2);

            if (t1 > tMin)
                tMin = t1;
            if (t2 < tMax)
                tMax = t2;

            if (tMin > tMax)
                return false;
        }

        return tMax >= std::max(tMin, 0.0f);
    }

    static bool MouseCast(const glm::vec2 &coords, CastData *data, float screenWidth, float screenHeight)
    {
        try
        {
            Scene *currentScene = SceneManager::get_current_scene();
            if (currentScene == nullptr)
                return false;

            Camera *camera = currentScene->main_camera;
            if (camera == nullptr)
                return false;

            bool is3D = camera->use_projection;

            float closestDistance = std::numeric_limits<float>::max();
            Entity *closestObject = nullptr;

            glm::vec3 rayDirection = CalculateRayDirection(coords, camera, screenWidth, screenHeight);
            rayDirection = glm::normalize(rayDirection);
            glm::vec3 rayOrigin = camera->cameraPosition;

            for (Entity *objD : currentScene->objects_worlds)
            {
                if (objD->get_transform() == nullptr)
                    continue;

                TransformComponent *transform = objD->get_transform();
                glm::vec3 posCam = transform->Position;
                glm::vec2 objPos2D = glm::vec2(posCam.x, posCam.y);
                float objWidth = transform->Scale.x;
                float objHeight = transform->Scale.y;
                float radians = transform->rotation.x;

                if (is3D)
                {
                    glm::mat4 modelMatrix = transform->get_matrix();
                    glm::mat4 invModelMatrix = glm::inverse(modelMatrix);

                    glm::vec3 localRayOrigin = glm::vec3(invModelMatrix * glm::vec4(rayOrigin, 1.0f));
                    glm::vec3 localRayDirection = glm::normalize(glm::vec3(invModelMatrix * glm::vec4(rayDirection, 0.0f)));

                    glm::vec3 localMin = glm::vec3(-objWidth / 2.0f, -objHeight / 2.0f, -transform->Scale.z / 2.0f);
                    glm::vec3 localMax = glm::vec3(objWidth / 2.0f, objHeight / 2.0f, transform->Scale.z / 2.0f);

                    float tMin, tMax;
                    bool intersect = RayAABBIntersection_Slab(localRayOrigin, localRayDirection, localMin, localMax, tMin, tMax);

                    std::cout << "Intersection with " << objD->ObjectName << ": " << (intersect ? "Yes" : "No")
                              << ", tMin: " << tMin << ", tMax: " << tMax << std::endl;

                    if (intersect)
                    {
                        if (tMin < closestDistance && tMin > 0.0f)
                        {
                            closestDistance = tMin;
                            closestObject = objD;
                            data->hitPoint = rayOrigin + tMin * rayDirection;
                        }
                    }
                }
                else
                {
                    glm::vec2 localPoint = RotatePoint(coords, objPos2D, radians);

                    if (localPoint.x >= (objPos2D.x - objWidth) && localPoint.x <= (objPos2D.x + objWidth) &&
                        localPoint.y >= (objPos2D.y - objHeight) && localPoint.y <= (objPos2D.y + objHeight))
                    {
                        float distance = posCam.z;
                        if (distance < closestDistance)
                        {
                            closestDistance = distance;
                            closestObject = objD;
                            data->hitPoint = glm::vec3(localPoint, posCam.z);
                        }
                    }
                }
            }

            if (closestObject != nullptr)
            {
                data->object = closestObject;
                std::cout << "Objeto más cercano seleccionado: " << closestObject->ObjectName << std::endl;
                return true;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error en MouseCast: " << e.what() << '\n';
        }

        return false;
    }

    static bool RayAABBIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir,
                                    const glm::vec3 &min, const glm::vec3 &max,
                                    float &tMin, float &tMax)
    {
        auto SafeDivide = [](float a, float b) -> float
        {
            if (b == 0.0f)
                return (a > 0.0f) ? std::numeric_limits<float>::max() : std::numeric_limits<float>::lowest();
            return a / b;
        };

        float invDirX = SafeDivide(1.0f, rayDir.x);
        float invDirY = SafeDivide(1.0f, rayDir.y);
        float invDirZ = SafeDivide(1.0f, rayDir.z);

        tMin = (min.x - rayOrigin.x) * invDirX;
        tMax = (max.x - rayOrigin.x) * invDirX;

        if (tMin > tMax)
            std::swap(tMin, tMax);

        float tyMin = (min.y - rayOrigin.y) * invDirY;
        float tyMax = (max.y - rayOrigin.y) * invDirY;

        if (tyMin > tyMax)
            std::swap(tyMin, tyMax);

        if ((tMin > tyMax) || (tyMin > tMax))
            return false;

        if (tyMin > tMin)
            tMin = tyMin;
        if (tyMax < tMax)
            tMax = tyMax;

        float tzMin = (min.z - rayOrigin.z) * invDirZ;
        float tzMax = (max.z - rayOrigin.z) * invDirZ;

        if (tzMin > tzMax)
            std::swap(tzMin, tzMax);

        if ((tMin > tzMax) || (tzMin > tMax))
            return false;

        if (tzMin > tMin)
            tMin = tzMin;
        if (tzMax < tMax)
            tMax = tzMax;

        return tMax > std::max(tMin, 0.0f);
    }

    static bool MouseCastUI(glm::vec2 coords, CastDataUI *data)
    {
        try
        {
            if (SceneManager::get_current_scene() != nullptr)
            {
                float closestZ = std::numeric_limits<float>::lowest();
                UIBehaviour *closestObject = nullptr;

                for (int i = 0; i < RenderPipeline::canvas->ui.size(); i++)
                {

                    UIBehaviour *objD = RenderPipeline::canvas->ui[i];

                    if (objD == nullptr)
                    {
                        return false;
                    }

                    glm::vec3 PosCam = objD->behaviour->Position;

                    glm::vec3 &obj = PosCam;
                    float objWidth = objD->behaviour->Scale.x;
                    float objHeight = objD->behaviour->Scale.y;

                    float radians = objD->behaviour->rotation.x;

                    glm::vec2 localPoint = RotatePoint(coords, obj, radians);

                    if (localPoint.x >= obj.x - objWidth && localPoint.x <= obj.x + objWidth &&
                        localPoint.y >= obj.y - objHeight && localPoint.y <= obj.y + objHeight)
                    {

                        if (obj.z > closestZ)
                        {
                            closestZ = obj.z;
                            closestObject = objD;
                        }
                    }
                }

                if (closestObject != nullptr)
                {
                    data->object = closestObject;
                    return true;
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        return false;
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

    static glm::vec2 RotatePoint(const glm::vec2 &point, const glm::vec2 &origin, float radians)
    {
        glm::vec2 translated = point - origin;
        float cosAngle = cos(-radians);
        float sinAngle = sin(-radians);
        glm::vec2 rotated(
            translated.x * cosAngle - translated.y * sinAngle,
            translated.x * sinAngle + translated.y * cosAngle);
        return rotated;
    }
};