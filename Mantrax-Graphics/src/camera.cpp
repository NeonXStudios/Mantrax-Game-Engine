#include "../includes/camera.h"
#include <Gfx.h>
#include <RenderPipeline.h>

void Camera::update()
{
    glm::mat4 rotationMatrix = glm::mat4_cast(cameraRotation);

    if (use_projection)
    {
        projection = glm::perspective(glm::radians(fov), (float)Gfx::render_width / (float)Gfx::render_height, 0.1f, distance_view);

        Orientation = glm::mat3(rotationMatrix) * glm::vec3(0.0f, 0.0f, 1.0f);

        view = glm::lookAt(cameraPosition, cameraPosition + Orientation, cameraUp);
        cameraMatrix = projection * view;
    }
    else
    {
        projection = glm::ortho(-Gfx::render_width / 2.0f * zoom, Gfx::render_width / 2.0f * zoom, -Gfx::render_height / 2.0f * zoom, Gfx::render_height / 2.0f * zoom, -1000.0f, 1000.0f);

        Orientation = glm::mat3(rotationMatrix) * glm::vec3(0.0f, 0.0f, -1.0f);

        view = glm::lookAt(cameraPosition, cameraPosition + Orientation, cameraUp);
        cameraMatrix = projection * view;

        cameraPosition.z = 10;
    }

    cameraUp = glm::mat4(rotationMatrix) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    cameraFront = glm::mat4(rotationMatrix) * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    cameraPosition += cameraVelocity;
    cameraVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
}
