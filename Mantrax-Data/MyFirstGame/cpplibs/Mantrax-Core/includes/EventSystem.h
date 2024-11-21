#pragma once
#include "GarinGraphics.h"
#include "Core.h"
#include <GarinMaths.h>

struct GARINLIBS_API CastData
{
    Entity *object;
};

class GARINLIBS_API EventSystem
{
public:
    static glm::vec2 ScreenToViewPort(glm::vec2 ViewportRenderPosition, glm::vec2 WindowSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        Camera *cam = SceneManager::GetOpenScene()->main_camera;

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
        Camera *cam = SceneManager::GetOpenScene()->main_camera;

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

    static bool MouseCast(glm::vec2 coords, CastData *data)
    {
        try
        {
        if (SceneManager::GetSceneManager()->OpenScene != nullptr){
            float closestZ = std::numeric_limits<float>::lowest();
            Entity *closestObject = nullptr;

            for (int i = 0; i < SceneManager::GetSceneManager()->OpenScene->objects_worlds.size(); i++)
            {

                Entity *objD = SceneManager::GetSceneManager()->OpenScene->objects_worlds[i];

                if (objD->get_transform() == nullptr)
                {
                    return false;
                }

                glm::vec3 PosCam = objD->get_transform()->Position;

                glm::vec3 &obj = PosCam;
                float objWidth = objD->get_transform()->Scale.x;
                float objHeight = objD->get_transform()->Scale.y;

                float radians = objD->get_transform()->rotation.x;

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

    static bool MouseCastNonScene(glm::vec2 coords, CastData *data, std::vector<Entity *> entitys)
    {
        float closestZ = std::numeric_limits<float>::lowest();
        Entity *closestObject = nullptr;

        for (int i = 0; i < entitys.size(); i++)
        {
            Entity *objD = entitys[i];

            glm::vec3 &obj = objD->get_transform()->Position;
            float objWidth = objD->get_transform()->Scale.x;
            float objHeight = objD->get_transform()->Scale.y;

            float radians = objD->get_transform()->rotation.x;

            glm::vec2 localPoint = RotatePoint(coords, obj, radians);

            // Solo considera la coordenada Z para determinar qué objeto está más adelante
            if (localPoint.x >= obj.x - objWidth && localPoint.x <= obj.x + objWidth &&
                localPoint.y >= obj.y - objHeight && localPoint.y <= obj.y + objHeight)
            {

                // Comprueba si la coordenada Z es mayor que la más cercana hasta ahora
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

        return false;
    }

    static glm::vec2 RotatePoint(const glm::vec2 &point, const glm::vec2 &center, float angle)
    {
        glm::vec2 rotatedPoint;
        float cosTheta = cos(angle);
        float sinTheta = sin(angle);

        rotatedPoint.x = center.x + (point.x - center.x) * cosTheta - (point.y - center.y) * sinTheta;
        rotatedPoint.y = center.y + (point.x - center.x) * sinTheta + (point.y - center.y) * cosTheta;

        return rotatedPoint;
    }
};