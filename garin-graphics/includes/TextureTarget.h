#pragma once
#ifndef TEXTURETARGET_CLASS
#define TEXTURETARGET_CLASS
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core.h>

class GARINLIBS_API TextureTarget
{
public:
    unsigned int texture;
    unsigned int framebuffer;
    unsigned int depthTexture;
    unsigned int colorAndDepthTexture;
    unsigned int depthRenderbuffer;

    int p_width = 1920;
    int p_height = 1080;

    void setup();
    void draw();
    unsigned int get_render();
};
#endif