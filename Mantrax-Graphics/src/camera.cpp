#include "../includes/camera.h"
#include <Gfx.h>
#include <RenderPipeline.h>
#include <EventSystem.h>

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


Ray Camera::ScreenToWorldRay(const glm::vec2& screenCoords)
{
    // Usar directamente las coordenadas del mouse convertidas en screen_to_viewport
    glm::vec2 viewportCoords = EventSystem::screen_to_viewport();  // Las coordenadas ya están en el sistema de la cámara

    // Convertir las coordenadas a NDC si es necesario para el cálculo (dependiendo de cómo las pases)
    float x = viewportCoords.x;
    float y = viewportCoords.y;

    std::cout << "X: " << x << std::endl;
    std::cout << "Y: " << y << std::endl;

    // Crear las coordenadas de clip space
    glm::vec4 clipCoords = glm::vec4(x, y, -1.0f, 1.0f);  // -1.0f para cámaras en perspectiva

    // Transformar de clip space a eye space (espacio de la cámara)
    glm::vec4 eyeCoords = glm::inverse(projection) * clipCoords;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);  // Z = -1.0f para el rayo

    // Convertir la dirección del rayo a world space (coordenadas del mundo)
    glm::vec3 worldDirection = glm::normalize(glm::vec3(glm::inverse(view) * eyeCoords));

    // Crear el rayo con la posición de la cámara y la dirección calculada
    Ray ray;
    ray.origin = cameraPosition;
    ray.direction = worldDirection;

    return ray;
}
