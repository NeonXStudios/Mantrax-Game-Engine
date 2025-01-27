#include "../includes/camera.h"
#include <Gfx.h>
#include <RenderPipeline.h>
#include <EventSystem.h>

void Camera::update()
{
    glm::mat4 rotationMatrix = glm::mat4_cast(cameraRotation);

    if (use_projection)
    {
        projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, distance_view);

        Orientation = glm::mat3(rotationMatrix) * glm::vec3(0.0f, 0.0f, 1.0f);

        view = glm::lookAt(cameraPosition, cameraPosition + Orientation, cameraUp);
        cameraMatrix = projection * view;
    }
    else
    {
        projection = glm::ortho(-width/ 2.0f * zoom, width / 2.0f * zoom, -height / 2.0f * zoom, height / 2.0f * zoom, -1000.0f, 1000.0f);

        Orientation = glm::mat3(rotationMatrix) * glm::vec3(0.0f, 0.0f, -1.0f);

        view = glm::lookAt(cameraPosition, cameraPosition + Orientation, cameraUp);
        cameraMatrix = projection * view;   
    }

    cameraUp = glm::mat4(rotationMatrix) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    cameraFront = glm::mat4(rotationMatrix) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    cameraPosition += cameraVelocity;
    cameraVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

Ray Camera::ScreenToWorldRay(const glm::vec2& screenCoords)
{
    glm::vec2 viewportCoords = EventSystem::mouse_to_view_port_position(glm::vec2(Gfx::render_width, Gfx::render_height));

    float x = (viewportCoords.x / (float)Gfx::render_width) * 2.0f - 1.0f;
    float y = -(viewportCoords.y / (float)Gfx::render_height) * 2.0f + 1.0f;

    glm::vec4 clipCoords = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, 1.0f, 0.0f); 

    glm::vec3 worldDirection = glm::normalize(glm::vec3(glm::inverse(view) * eyeCoords));

    Ray ray;
    ray.origin = cameraPosition;
    ray.direction = worldDirection;

    return ray;
}
