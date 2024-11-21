#include "../includes/InputSystem.h"

double InputSystem::lastX = 0.0;
double InputSystem::lastY = 0.0;

double InputSystem::mouse_delta_x = 0.0;
double InputSystem::mouse_delta_y = 0.0;

std::unordered_map<GLuint, bool> InputSystem::current_key_state;
std::unordered_map<GLuint, bool> InputSystem::previous_key_state;

void InputSystem::initialize()
{
    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        current_key_state[i] = false;
        previous_key_state[i] = false;
    }
}

bool InputSystem::on_key_down(GLuint key)
{
    bool is_down = current_key_state[key];
    bool was_down = previous_key_state[key];
    return is_down && !was_down;
}

float InputSystem::get_axis(GLuint positive_key, GLuint negative_key)
{
    return -static_cast<float>(InputSystem::on_key_pressed(negative_key)) + static_cast<float>(InputSystem::on_key_pressed(positive_key));
}

bool InputSystem::on_key_pressed(GLuint key)
{
    return glfwGetKey(Gfx::get_game_window(), key) == GLFW_PRESS;
}

void InputSystem::update_input()
{
    float abstract_sense = 0.1f;
    double mouseX, mouseY;
    glfwGetCursorPos(Gfx::get_game_window(), &mouseX, &mouseY);

    float deltaX = static_cast<float>(mouseX - lastX);
    float deltaY = static_cast<float>(lastY - mouseY);

    lastX = mouseX;
    lastY = mouseY;

    mouse_delta_x = deltaX * abstract_sense;
    mouse_delta_y = deltaY * abstract_sense;

    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        previous_key_state[i] = current_key_state[i];
    }

    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        current_key_state[i] = glfwGetKey(Gfx::get_game_window(), i) == GLFW_PRESS;
    }
}

float InputSystem::get_mouse_x()
{
    return mouse_delta_x;
}

float InputSystem::get_mouse_y()
{
    return mouse_delta_y;
}

// Funciones para el joystick
bool InputSystem::is_joystick_connected(int joystick_id)
{
    return glfwJoystickPresent(joystick_id) == GLFW_TRUE;
}

const float *InputSystem::get_joystick_axes(int joystick_id, int *count)
{
    if (is_joystick_connected(joystick_id))
    {
        return glfwGetJoystickAxes(joystick_id, count);
    }
    *count = 0;
    return nullptr;
}

const unsigned char *InputSystem::get_joystick_buttons(int joystick_id, int *count)
{
    if (is_joystick_connected(joystick_id))
    {
        return glfwGetJoystickButtons(joystick_id, count);
    }
    *count = 0;
    return nullptr;
}

const char *InputSystem::get_joystick_name(int joystick_id)
{
    if (is_joystick_connected(joystick_id))
    {
        return glfwGetJoystickName(joystick_id);
    }
    return "No conectado";
}

// extern "C" GARINLIBS_API bool is_key_down(GLuint key)
// {
//     return InputSystem::on_key_down(key);
// }

// extern "C" GARINLIBS_API float get_mouse_delta_x()
// {
//     return InputSystem::get_mouse_x();
// }

// extern "C" GARINLIBS_API float get_mouse_delta_y()
// {
//     return InputSystem::get_mouse_y();
// }
