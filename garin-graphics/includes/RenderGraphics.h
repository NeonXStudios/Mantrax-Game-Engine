#pragma once
#include <functional>
#include <Drawer.h>

using namespace std;

class RenderGraphics
{
public:
    void checkFramebufferStatus();
    void start();
    void update(std::function<void(void)> func, bool target_render);
    ~RenderGraphics();

public:
    unsigned int texture;
    unsigned int framebuffer;   
    unsigned int depthTexture;  
    GLuint colorAndDepthTexture; 
    GLuint depthRenderbuffer; 


    void checkGLError(const char* location);
};