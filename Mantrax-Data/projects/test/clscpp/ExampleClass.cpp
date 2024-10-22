#include "ExampleClass.h"

void ExampleClass::on_init()
{
    std::cout << "Self Object: " << self->ObjectName << std::endl;
}

void ExampleClass::on_tick()
{

    direction = glm::vec3((InputSystem::get_axis(GLFW_KEY_D, GLFW_KEY_A) * move_speed) * Timer::delta_time, 0.0f, 0.0f);
    direction.y = -1.0f * Timer::delta_time;

    if (InputSystem::on_key_pressed(GLFW_KEY_SPACE))
    {
        direction.y = 1.81f;
    }

    if (self->hasComponent<GCharacter>())
    {
        self->getComponent<GCharacter>().move(direction);
    }

    try
    {
        // find_object_by_name("b")->getComponent<GCollision>().setLayerMask(LAYER_0);
        // find_object_by_name("b")->get_transform()->Position.z = 0;

        GCastHit hit;
        GCaster *ray = new GCaster();

        if (ray->RayCast(self->get_transform()->Position, glm::vec3(0.0f, -1.0f, 0.0f), 50.0f, &hit, PxHitFlags(PxHitFlag::eDEFAULT), LAYER_0 | LAYER_1))
        {
            std::cout << "Object Cast: " << hit.entity->ObjectName << std::endl;
        }

        if (ray->RayCast(self->get_transform()->Position, glm::vec3(0.0f, 1.0f, 0.0f), 50.0f, &hit, PxHitFlags(PxHitFlag::eDEFAULT), LAYER_0 | LAYER_1))
        {
            std::cout << "Object Cast: " << hit.entity->ObjectName << std::endl;
        }

        if (ray->RayCast(self->get_transform()->Position, glm::vec3(1.0f, 0.0f, 0.0f), 50.0f, &hit, PxHitFlags(PxHitFlag::eDEFAULT), LAYER_0 | LAYER_1))
        {
            std::cout << "Object Cast: " << hit.entity->ObjectName << std::endl;
        }

        if (ray->RayCast(self->get_transform()->Position, glm::vec3(0.0f, 0.0f, 1.0f), 50.0f, &hit, PxHitFlags(PxHitFlag::eDEFAULT), LAYER_0 | LAYER_1))
        {
            std::cout << "Object Cast: " << hit.entity->ObjectName << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    float deltaTime = 0.016f;

    Graphics::get_main_camera()->cameraPosition = glm::mix(Graphics::get_main_camera()->cameraPosition, self->get_transform()->Position, interpolationSpeed * deltaTime);

    // // std::cout << "OBJETOS EN ESCENA: " << Graphics::get_current_scene()->objects_worlds.size() << std::endl;
}