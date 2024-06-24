#include "../includes/InputSystem.h"

double InputSystem::lastX = 0.0;
double InputSystem::lastY = 0.0;

double InputSystem::mouse_delta_x = 0.0;
double InputSystem::mouse_delta_y = 0.0;

float InputSystem::get_axis(GLuint positive_key, GLuint negative_key)
{
    return -static_cast<float>(InputSystem::on_key_pressed(negative_key)) + static_cast<float>(InputSystem::on_key_pressed(positive_key));
}

bool InputSystem::on_key_pressed(GLuint key)
{
    return glfwGetKey(Graphics::get_game_window(), key) == GLFW_PRESS;
}

void InputSystem::update_input()
{
    float abstract_sense = 0.1f;
    double mouseX, mouseY;
    glfwGetCursorPos(Graphics::get_game_window(), &mouseX, &mouseY);

    float deltaX = static_cast<float>(mouseX - lastX);
    float deltaY = static_cast<float>(lastY - mouseY);

    lastX = mouseX;
    lastY = mouseY;

    mouse_delta_x = deltaX * abstract_sense;
    mouse_delta_y = deltaY * abstract_sense;
}

float InputSystem::get_mouse_x()
{
    // float sensitivity = 0.2f;
    // double mouseX, mouseY;
    // glfwGetCursorPos(Graphics::get_game_window(), &mouseX, &mouseY);

    // float deltaX = static_cast<float>(mouseX - lastX);
    // lastX = mouseX;

    return mouse_delta_x;
}

// Obtiene el desplazamiento del mouse en el eje Y desde el último cuadro
float InputSystem::get_mouse_y()
{
    // float sensitivity = 0.2f;
    // double mouseX, mouseY;
    // glfwGetCursorPos(Graphics::get_game_window(), &mouseX, &mouseY);

    // float deltaY = static_cast<float>(lastY - mouseY);
    // lastY = mouseY;

    return mouse_delta_y;
}

// glm::vec2 InputSystem::get_mouse()
// {
//     float sensitivity = 0.2f;
//     double mouseX, mouseY;
//     glfwGetCursorPos(Graphics::get_game_window(), &mouseX, &mouseY);

//     float deltaX = static_cast<float>(mouseX - lastX);
//     float deltaY = static_cast<float>(lastY - mouseY);

//     lastX = mouseX;
//     lastY = mouseY;

//     float rotX = deltaY * sensitivity;
//     float rotY = deltaX * sensitivity;

//     return glm::vec2(rotY, rotX);
// }
