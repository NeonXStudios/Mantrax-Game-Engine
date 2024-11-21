#pragma once
#include <GarinGraphics.h>

class EngineWrapper
{
public:
    struct GLFWWrapper
    {
        GLFWwindow *window;
    };

    static GLFWWrapper wrapper;
};
