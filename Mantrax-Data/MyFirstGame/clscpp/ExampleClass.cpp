#include "ExampleClass.h"

void ExampleClass::on_init()
{

}

void ExampleClass::on_tick()
{
    if (InputSystem::on_key_down(GLFW_KEY_E)){
        std::cout << InputSystem::get_mouse_x() << std::endl;
    }
}