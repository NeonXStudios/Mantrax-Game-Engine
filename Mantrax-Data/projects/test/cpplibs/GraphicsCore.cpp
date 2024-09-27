#include "GraphicsCore.h"

Graphics *GraphicsCore::graphics_core = nullptr;
unsigned int GraphicsCore::get_render()
{
    return Graphics::get_render();
}

int GraphicsCore::get_width()
{
    return Graphics::get_width();
}

int GraphicsCore::get_height()
{
    return Graphics::get_height();
}

void GraphicsCore::set_vsync(bool set_vsync)
{
    Graphics::set_vsync(set_vsync);
}

void GraphicsCore::set_screen(int index_screen)
{
    Graphics::set_screen(index_screen);
}