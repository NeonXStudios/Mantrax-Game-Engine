#include "ExampleClass.h"

void ExampleClass::on_init()
{
    // Inicialización
}

void ExampleClass::on_tick()
{
    std::cout << "OBJETOS EN ESCENA: " << Graphics::get_current_scene()->objects_worlds.size() << std::endl;
}