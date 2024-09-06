#include "../includes/camera.h"
#include <Gfx.h>
#include <RenderPipeline.h>

void Camera::update()
{
    float rotationXAngle360 = fmod(rotationXAngle, 360);
    float rotationYAngle360 = fmod(rotationYAngle, 360);
    float rotationZAngle360 = fmod(rotationZAngle, 360);

    glm::quat rotationX = glm::angleAxis(glm::radians(rotationXAngle360), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat rotationY = glm::angleAxis(glm::radians(rotationYAngle360), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotationZ = glm::angleAxis(glm::radians(rotationZAngle360), glm::vec3(0.0f, 0.0f, 1.0f));

    cameraRotation = rotationX * rotationY * rotationZ;

    glm::mat4 rotationMatrix = glm::mat4_cast(cameraRotation);

    if (use_projection)
    {
        view = glm::lookAt(cameraPosition, cameraPosition + Orientation, cameraUp);
        projection = glm::perspective(glm::radians(fov), (float)Gfx::render_width / (float)Gfx::render_height, 0.1f, 10000.0f);
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

Camera::Camera()
{
    RenderPipeline::camera_targets.push_back(this);
}