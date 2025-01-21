#include <EventSystem.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>


glm::vec2 EventSystem::ViewportRenderPosition = glm::vec2(0.0f, 0.0f);

    glm::vec2 EventSystem::screen_to_viewport() {
        double x, y;
        glfwGetCursorPos(Gfx::get_game_window(), &x, &y);
        
        Camera *cam = SceneManager::get_current_scene()->main_camera;
        
        float viewportX = x - ViewportRenderPosition.x;
        float viewportY = y - ViewportRenderPosition.y;
        
        if (viewportX < 0 || viewportX > Gfx::render_width ||
            viewportY < 0 || viewportY > Gfx::render_height) {
            return glm::vec2(0.0f, 0.0f);
        }
        
        float normalizedX = viewportX / static_cast<float>(Gfx::render_width);
        float normalizedY = viewportY / static_cast<float>(Gfx::render_height);
        
        float ndcX = (normalizedX * 2.0f) - 1.0f;
        float ndcY = 1.0f - (normalizedY * 2.0f);
        
        // float aspectRatio = static_cast<float>(Gfx::render_width) / static_cast<float>(Gfx::render_height);
        // ndcX *= aspectRatio;
        
        return glm::vec2(ndcX, ndcY);
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

    // bool EventSystem::MouseCast2D(glm::vec2 mouseCoords, CastData *data) 
    // { 
    //     const float MIN_PICK_DISTANCE = 0.1f; 
    //     const float MAX_PICK_DISTANCE = 1000.0f; 
    //     const float EPSILON = 0.0001f; 
    //     float closestDistance = MAX_PICK_DISTANCE; 
    //     Entity* closestObject = nullptr; 

    //     Camera* camera = SceneManager::get_current_scene()->main_camera; 
    //     glm::mat4 viewMatrix = glm::inverse(camera->GetViewInverse()); 
    //     glm::mat4 projectionMatrix = camera->GetProjectionMatrix(); 

    //     glm::vec3 rayOrigin, rayDirection; 
    //     ScreenToWorldRay(mouseCoords, glm::inverse(viewMatrix), glm::inverse(projectionMatrix), rayOrigin, rayDirection); 

    //     for (Entity* objD : SceneManager::get_current_scene()->objects_worlds) 
    //     { 
    //         glm::vec3 objPosition = objD->get_transform()->getPosition(); 
    //         glm::vec3 objScale = objD->get_transform()->Scale; 
    //         glm::quat objRotation = objD->get_transform()->rotation; 

    //         glm::mat4 modelNoScale = glm::translate(glm::mat4(1.0f), objPosition) * 
    //                                 glm::mat4_cast(objRotation);
    //         glm::mat4 inverseModelNoScale = glm::inverse(modelNoScale);

    //         const float BASE_SIZE = 1.0f;
    //         glm::vec3 baseMin = glm::vec3(-BASE_SIZE);
    //         glm::vec3 baseMax = glm::vec3(BASE_SIZE);

    //         glm::vec3 localRayOrigin = glm::vec3(inverseModelNoScale * glm::vec4(rayOrigin, 1.0f));
    //         glm::vec3 localRayDirection = glm::normalize(glm::vec3(inverseModelNoScale * glm::vec4(rayDirection, 0.0f)));

    //         localRayOrigin /= objScale;
    //         localRayDirection = glm::normalize(localRayDirection / objScale);

    //         bool isInside = (localRayOrigin.x >= baseMin.x && localRayOrigin.x <= baseMax.x && 
    //                         localRayOrigin.y >= baseMin.y && localRayOrigin.y <= baseMax.y && 
    //                         localRayOrigin.z >= baseMin.z && localRayOrigin.z <= baseMax.z); 

    //         if (isInside || RayIntersectsAABB(localRayOrigin, localRayDirection, baseMin, baseMax)) 
    //         { 
    //             float distance = 0.0f; 

    //             if (!isInside) { 
    //                 float tx1 = (baseMin.x - localRayOrigin.x) / localRayDirection.x; 
    //                 float tx2 = (baseMax.x - localRayOrigin.x) / localRayDirection.x; 
    //                 float ty1 = (baseMin.y - localRayOrigin.y) / localRayDirection.y; 
    //                 float ty2 = (baseMax.y - localRayOrigin.y) / localRayDirection.y; 
    //                 float tz1 = (baseMin.z - localRayOrigin.z) / localRayDirection.z; 
    //                 float tz2 = (baseMax.z - localRayOrigin.z) / localRayDirection.z; 

    //                 float txmin = (tx1 < tx2) ? tx1 : tx2;
    //                 float txmax = (tx1 > tx2) ? tx1 : tx2;
    //                 float tymin = (ty1 < ty2) ? ty1 : ty2;
    //                 float tymax = (ty1 > ty2) ? ty1 : ty2;
    //                 float tzmin = (tz1 < tz2) ? tz1 : tz2;
    //                 float tzmax = (tz1 > tz2) ? tz1 : tz2;

    //                 float tmin = max3(txmin, tymin, tzmin);
    //                 float tmax = min3(txmax, tymax, tzmax);

    //                 if (tmax >= tmin) {
    //                     glm::vec3 localIntersection = localRayOrigin + localRayDirection * tmin;
    //                     localIntersection *= objScale;
    //                     glm::vec3 worldIntersection = glm::vec3(modelNoScale * glm::vec4(localIntersection, 1.0f));
    //                     distance = glm::distance(rayOrigin, worldIntersection);
    //                 }
    //             } 

    //             if (distance < closestDistance - EPSILON) 
    //             { 
    //                 closestDistance = distance; 
    //                 closestObject = objD; 
    //             } 
    //         } 
    //     } 

    //     if (closestObject != nullptr && closestDistance >= MIN_PICK_DISTANCE && closestDistance <= MAX_PICK_DISTANCE) 
    //     { 
    //         data->object = closestObject; 
    //         return true; 
    //     } 

    //     return false; 
    // }

    bool EventSystem::MouseCast2D(glm::vec2 mouseCoords, CastData* data) {
        const float MIN_PICK_DISTANCE = 0.1f;
        const float MAX_PICK_DISTANCE = 1000.0f;
        const float EPSILON = 0.0001f;
        float closestDistance = MAX_PICK_DISTANCE;
        Entity* closestObject = nullptr;

        Camera* camera = SceneManager::get_current_scene()->main_camera;
        glm::mat4 viewMatrix = glm::inverse(camera->GetViewInverse());
        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

        glm::vec3 rayOrigin, rayDirection;
        ScreenToWorldRay(mouseCoords, glm::inverse(viewMatrix), glm::inverse(projectionMatrix), rayOrigin, rayDirection);

        for (Entity* objD : SceneManager::get_current_scene()->objects_worlds) {
            glm::vec3 objPosition = objD->get_transform()->getPosition();
            glm::vec3 objScale = objD->get_transform()->Scale;
            glm::quat objRotation = objD->get_transform()->rotation;

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), objPosition) *
                glm::mat4_cast(objRotation) *
                glm::scale(glm::mat4(1.0f), objScale);

            glm::mat4 inverseModel = glm::inverse(modelMatrix);

            glm::vec3 localRayOrigin = glm::vec3(inverseModel * glm::vec4(rayOrigin, 1.0f));
            glm::vec3 localRayDirection = glm::normalize(glm::vec3(inverseModel * glm::vec4(rayDirection, 0.0f)));

            if (objD->hasComponent<ModelComponent>()) {
                Model* model = objD->getComponent<ModelComponent>().model;

                for (Mesh& mesh : model->meshes) {
                    const std::vector<Vertex>& vertices = mesh.vertices;
                    const std::vector<unsigned int>& indices = mesh.indices;

                    for (size_t i = 0; i < indices.size(); i += 3) {
                        glm::vec3 v0 = vertices[indices[i]].Position;
                        glm::vec3 v1 = vertices[indices[i + 1]].Position;
                        glm::vec3 v2 = vertices[indices[i + 2]].Position;

                        float t = 0.0f;
                        if (RayIntersectsTriangle(localRayOrigin, localRayDirection, v0, v1, v2, t)) {
                            glm::vec3 hitPoint = localRayOrigin + localRayDirection * t;
                            glm::vec3 worldHitPoint = glm::vec3(modelMatrix * glm::vec4(hitPoint, 1.0f));
                            float worldDistance = glm::length(worldHitPoint - rayOrigin);

                            if (worldDistance < closestDistance - EPSILON) {
                                closestDistance = worldDistance;
                                closestObject = objD;
                            }
                        }
                    }
                }
            }
        }

        if (closestObject != nullptr && closestDistance >= MIN_PICK_DISTANCE && closestDistance <= MAX_PICK_DISTANCE) {
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