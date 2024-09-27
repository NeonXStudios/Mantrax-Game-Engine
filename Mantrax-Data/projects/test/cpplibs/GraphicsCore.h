#pragma once
#include <GarinNatives.h>
#include <GarinGraphics.h>

class GraphicsCore
{
public:
    static Graphics *graphics_core;
    static GLFWwindow *window;
    static unsigned int get_render();
    static int get_width();
    static int get_height();
    static void set_vsync(bool set_vsync);
    static void set_screen(int index_screen);
};