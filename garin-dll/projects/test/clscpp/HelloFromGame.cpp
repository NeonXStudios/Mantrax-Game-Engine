#include "HelloFromGame.h"

void HelloFromGame::on_init()
{
    // Inicialización
}

void HelloFromGame::on_tick()
{
    std::cout << "OBJETOS EN ESCENA: " << Graphics::get_current_scene()->objects_worlds.size() << std::endl;
}