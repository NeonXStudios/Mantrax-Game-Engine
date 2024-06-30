#include "ExampleGame.h"

void ExampleGame::on_init()
{
    // Inicialización
}

void ExampleGame::on_tick()
{
    std::cout << "OBJETOS EN ESCENA: " << Graphics::get_current_scene()->objects_worlds.size() << std::endl;
}