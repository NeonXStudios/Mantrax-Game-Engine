#include <EventSystem.h>
#include <limits>
#include <glm/glm.hpp>

glm::vec2 EventSystem::ViewportRenderPosition = glm::vec2(0.0f, 0.0f);

    glm::vec2 EventSystem::screen_to_viewport()
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        Camera *cam = SceneManager::get_current_scene()->main_camera;

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = (windowMousePosX / Gfx::render_width);
        double NormalMousePosY = (-windowMousePosY / Gfx::render_height);

        double centeredMousePosX = ((NormalMousePosX * 2.0f - 1.0f) * (Gfx::render_width / 2) * cam->zoom);
        double centeredMousePosY = ((NormalMousePosY * 2.0f + 1.0f) * (Gfx::render_height / 2) * cam->zoom);

        double WorldPointX = centeredMousePosX + cam->cameraPosition.x;
        double WorldPointY = centeredMousePosY + cam->cameraPosition.y;

        return glm::vec2(WorldPointX, WorldPointY);
    }

    glm::vec2 EventSystem::get_mouse_position_in_viewport(glm::vec2 WindowSize, glm::vec2 ScreenSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);

        double windowMousePosX = x - ViewportRenderPosition.x;
        double windowMousePosY = y - ViewportRenderPosition.y;

        double NormalMousePosX = windowMousePosX / WindowSize.x;
        double NormalMousePosY = -windowMousePosY / WindowSize.y;

        return vec2(0, 0);
    }

    glm::vec2 EventSystem::mouse_to_view_port_position(glm::vec2 WindowSize)
    {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);
        Camera *cam = SceneManager::get_current_scene()->main_camera;

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

    glm::vec2 EventSystem::mouse_to_screen_pos(glm::vec2 WindowSize)
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

    bool EventSystem::MouseCast2D(glm::vec2 coords, CastData *data)
    {
        float closestZ = std::numeric_limits<float>::lowest();
        Entity *closestObject = nullptr;

        for (int i = 0; i < SceneManager::get_current_scene()->objects_worlds.size(); i++)
        {
            Entity *objD = SceneManager::get_current_scene()->objects_worlds[i];

            glm::vec3 &obj = objD->get_transform()->getPosition();

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

        return false;
    }

    bool EventSystem::MouseCast3D(const glm::vec2& screenCoords, CastData* data)
    {
        Camera* camera = SceneManager::get_current_scene()->main_camera;

        glm::vec3 rayOrigin = camera->cameraPosition;  
        glm::vec3 rayDirection = camera->ScreenToWorldRay(screenCoords).direction; 

        float closestZ = std::numeric_limits<float>::lowest();
        Entity* closestObject = nullptr;

        // Recorrer todos los objetos en la escena para encontrar la intersección
        for (int i = 0; i < SceneManager::get_current_scene()->objects_worlds.size(); i++)
        {
            Entity* objD = SceneManager::get_current_scene()->objects_worlds[i];

            glm::vec3& objPosition = objD->get_transform()->getPosition();
            glm::vec3 objScale = objD->get_transform()->Scale;

            glm::vec3 min = objPosition - objScale / 2.0f;
            glm::vec3 max = objPosition + objScale / 2.0f;
            
            float tmin;

            if (RayIntersectsAABB(rayOrigin, rayDirection, min, max, &tmin))
            {
                if (objPosition.z > closestZ)
                {
                    closestZ = objPosition.z;
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


    bool EventSystem::MouseCastUI(glm::vec2 coords, CastDataUI *data)
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

    glm::vec2 EventSystem::RotatePoint(const glm::vec2 &point, const glm::vec2 &center, float angle)
    {
        glm::vec2 rotatedPoint;
        float cosTheta = cos(angle);
        float sinTheta = sin(angle);

        rotatedPoint.x = center.x + (point.x - center.x) * cosTheta - (point.y - center.y) * sinTheta;
        rotatedPoint.y = center.y + (point.x - center.x) * sinTheta + (point.y - center.y) * cosTheta;

        return rotatedPoint;
    }